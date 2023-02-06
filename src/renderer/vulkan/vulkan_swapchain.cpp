#include "vulkan/vulkan_swapchain.h"

#include "vulkan/vulkan_device.h"
#include "vulkan/vulkan_physical_device.h"
#include "vulkan/vulkan_surface.h"
#include "vulkan/vulkan_render_pass.h"
#include "vulkan/vulkan_image.h"
#include "window.h"

#include <array>

namespace glibby
{
	VulkanSwapchain::VulkanSwapchain(VulkanDevice* device, VulkanPhysicalDevice* physicalDevice, VulkanSurface* surface, VulkanRenderPass* renderPass)
	{
		this->device = device;
		this->physicalDevice = physicalDevice;
		this->surface = surface;
		this->renderPass = renderPass;

		CreateSwapchain();
		CreateImageViews();
		CreateColorResources();
		CreateDepthResources();
		CreateFramebuffers();
	}

	void VulkanSwapchain::CreateSwapchain()
	{
		SwapchainSupportDetails swapchainSupport = QuerySwapchainSupport(physicalDevice->GetHandle(), surface);

		VkSurfaceFormatKHR surfaceFormat = physicalDevice->ChooseSwapSurfaceFormat(swapchainSupport.formats);
		VkPresentModeKHR presentMode = physicalDevice->ChooseSwapPresentMode(swapchainSupport.presentModes);
		VkExtent2D extent = physicalDevice->ChooseSwapExtent(swapchainSupport.capabilities, surface);

		uint32_t imageCount = swapchainSupport.capabilities.minImageCount + 1;
		if (swapchainSupport.capabilities.maxImageCount > 0 && imageCount > swapchainSupport.capabilities.maxImageCount) {
			imageCount = swapchainSupport.capabilities.maxImageCount;
		}
		
		VkSwapchainCreateInfoKHR createInfo = { VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR };
		createInfo.surface = surface->GetHandle();
		createInfo.minImageCount = imageCount;
		createInfo.imageFormat = surfaceFormat.format;
		createInfo.imageColorSpace = surfaceFormat.colorSpace;
		createInfo.imageExtent = extent;
		createInfo.imageArrayLayers = 1;
		createInfo.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;

		uint32_t queueFamilyIndexArray[] = 
		{
			physicalDevice->queueFamilyIndices.graphicsFamily.value(), 
			physicalDevice->queueFamilyIndices.presentFamily.value()
		};

		if (physicalDevice->queueFamilyIndices.graphicsFamily != physicalDevice->queueFamilyIndices.presentFamily)
		{
			createInfo.imageSharingMode = VK_SHARING_MODE_CONCURRENT;
			createInfo.queueFamilyIndexCount = 2;
			createInfo.pQueueFamilyIndices = queueFamilyIndexArray;
		}
		else
		{
			createInfo.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
			createInfo.queueFamilyIndexCount = 0; // Optional
			createInfo.pQueueFamilyIndices = nullptr; // Optional
		}

		createInfo.preTransform = swapchainSupport.capabilities.currentTransform;
		createInfo.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
		createInfo.presentMode = presentMode;
		createInfo.clipped = VK_TRUE;
		createInfo.oldSwapchain = VK_NULL_HANDLE;

		VK_CHECK(vkCreateSwapchainKHR(device->GetHandle(), &createInfo, nullptr, &handle), "Failed to create swapchain.");

		vkGetSwapchainImagesKHR(device->GetHandle(), handle, &imageCount, nullptr);
		swapchainImages.resize(imageCount);
		vkGetSwapchainImagesKHR(device->GetHandle(), handle, &imageCount, swapchainImages.data());

		swapchainImageFormat = surfaceFormat.format;
		swapchainExtent = extent;
	}

	VulkanSwapchain::~VulkanSwapchain()
	{
		CleanupSwapchain();
	}

	void VulkanSwapchain::CleanupSwapchain()
	{
		colorImage->~VulkanImage();
		depthImage->~VulkanImage();

		for (size_t i = 0; i < swapchainFramebuffers.size(); i++)
		{
			vkDestroyFramebuffer(device->GetHandle(), swapchainFramebuffers[i], nullptr);
		}

		for (size_t i = 0; i < swapchainImageViews.size(); i++)
		{
			vkDestroyImageView(device->GetHandle(), swapchainImageViews[i], nullptr);
		}

		vkDestroySwapchainKHR(device->GetHandle(), handle, nullptr);
	}

	void VulkanSwapchain::RecreateSwapchain()
	{
		int width = 0, height = 0;
		Window* window = surface->GetWindow();

		window->GetFramebufferSize(&width, &height);
		while (width == 0 || height == 0)
		{
			window->GetFramebufferSize(&width, &height);
			window->WaitEvents();
		}

		vkDeviceWaitIdle(device->GetHandle());

		CleanupSwapchain();

		CreateSwapchain();
		CreateImageViews();
		CreateColorResources();
		CreateDepthResources();
		CreateFramebuffers();
	}

	void VulkanSwapchain::CreateImageViews()
	{
		swapchainImageViews.resize(swapchainImages.size());
		for (size_t i = 0; i < swapchainImages.size(); i++)
		{
			swapchainImageViews[i] = CreateImageView(device->GetHandle(), swapchainImages[i], swapchainImageFormat, VK_IMAGE_ASPECT_COLOR_BIT, 1);
		}
	}

	void VulkanSwapchain::CreateColorResources()
	{
		VkFormat colorFormat = swapchainImageFormat;

		colorImage = new VulkanImage(device,
			swapchainExtent.width, swapchainExtent.height, 1, device->GetMaxUsableSampleCount(), colorFormat,
			VK_IMAGE_TILING_OPTIMAL, VK_IMAGE_USAGE_TRANSIENT_ATTACHMENT_BIT | VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT,
			VK_IMAGE_ASPECT_COLOR_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT);
	}

	void VulkanSwapchain::CreateDepthResources()
	{
		VkFormat depthFormat = device->FindDepthFormat();

		depthImage = new VulkanImage(device,
			swapchainExtent.width, swapchainExtent.height, 1, device->GetMaxUsableSampleCount(), depthFormat,
			VK_IMAGE_TILING_OPTIMAL, VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT,
			VK_IMAGE_ASPECT_DEPTH_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT);

		// This is not necessary, it is done automatically by the render pass:
		// TransitionImageLayout(depthImage, depthFormat, VK_IMAGE_LAYOUT_UNDEFINED, VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL, 1);
	}

	void VulkanSwapchain::CreateFramebuffers()
	{
		swapchainFramebuffers.resize(swapchainImageViews.size());

		for (size_t i = 0; i < swapchainImageViews.size(); i++) {
			std::array<VkImageView, 3> attachments = {
				colorImage->GetImageView(),
				depthImage->GetImageView(),
				swapchainImageViews[i],
			};

			VkFramebufferCreateInfo framebufferInfo = { VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO };
			framebufferInfo.renderPass = renderPass->GetHandle();
			framebufferInfo.attachmentCount = static_cast<uint32_t>(attachments.size());
			framebufferInfo.pAttachments = attachments.data();
			framebufferInfo.width = swapchainExtent.width;
			framebufferInfo.height = swapchainExtent.height;
			framebufferInfo.layers = 1;

			VK_CHECK(vkCreateFramebuffer(device->GetHandle(), &framebufferInfo, nullptr, &swapchainFramebuffers[i]), "Failed to create framebuffer.");
		}
	}
}
