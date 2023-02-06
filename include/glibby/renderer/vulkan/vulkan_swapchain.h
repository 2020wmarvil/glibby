#pragma once

#include "renderer_core.h"

#include <vector>
#include <memory>

namespace glibby
{
	class VulkanDevice;
	class VulkanPhysicalDevice;
	class VulkanSurface;
	class VulkanRenderPass;
	class VulkanImage;

	class VulkanSwapchain
	{
	public:
		VulkanSwapchain(VulkanDevice* device, VulkanPhysicalDevice* physicalDevice, VulkanSurface* surface, VulkanRenderPass* renderPass);
		~VulkanSwapchain();

		VkSwapchainKHR GetHandle() { return handle; }
		void RecreateSwapchain();
		void GetSize(uint32_t* width, uint32_t* height) { *width = swapchainExtent.width; *height = swapchainExtent.height; }
		VkExtent2D GetExtent() { return swapchainExtent; }
		VkFramebuffer GetFramebuffer(uint32_t imageIndex) { return swapchainFramebuffers[imageIndex]; }
		VulkanRenderPass* GetRenderPass() { return renderPass; }

	private:
		void CreateSwapchain();
		void CleanupSwapchain();

		void CreateImageViews();
		void CreateColorResources();
		void CreateDepthResources();
		void CreateFramebuffers();

	private:
		VulkanDevice* device;
		VulkanPhysicalDevice* physicalDevice;
		VulkanSurface* surface;
		VulkanRenderPass* renderPass;
		VkSwapchainKHR handle;

		VulkanImage* colorImage;
		VulkanImage* depthImage;

		std::vector<VkImage> swapchainImages;
		std::vector<VkImageView> swapchainImageViews;
		VkFormat swapchainImageFormat;
		VkExtent2D swapchainExtent;
		std::vector<VkFramebuffer> swapchainFramebuffers;
	};
}

