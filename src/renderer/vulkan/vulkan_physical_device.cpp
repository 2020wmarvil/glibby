#include "vulkan/vulkan_physical_device.h"

#include "vulkan/vulkan_instance.h"
#include "vulkan/vulkan_queue.h"
#include "vulkan/vulkan_swapchain.h"
#include "vulkan/vulkan_surface.h"
#include "window.h"

#include <vector>
#include <unordered_set>
#include <algorithm>

namespace glibby
{

	VulkanPhysicalDevice::VulkanPhysicalDevice(VulkanInstance* instance, const VulkanPhysicalDeviceRequirements& physicalDeviceRequirements)
		: instance(instance)
	{
		handle = PickPhysicalDevice(physicalDeviceRequirements);
		queueFamilyIndices = FindQueueFamilies(handle, physicalDeviceRequirements.surface->GetHandle());

		vkGetPhysicalDeviceProperties(handle, &properties);
		vkGetPhysicalDeviceFeatures(handle, &features);
	}

	VkPhysicalDevice VulkanPhysicalDevice::PickPhysicalDevice(const VulkanPhysicalDeviceRequirements& requirements) const
	{
		uint32_t deviceCount = 0;
		vkEnumeratePhysicalDevices(instance->GetHandle(), &deviceCount, nullptr);
		std::vector<VkPhysicalDevice> devices(deviceCount);
		vkEnumeratePhysicalDevices(instance->GetHandle(), &deviceCount, devices.data());

		std::vector<VkPhysicalDevice> validDevices;
		for (const VkPhysicalDevice& physicalDevice : devices) {
			VulkanQueueFamilyIndices indices = FindQueueFamilies(physicalDevice, requirements.surface->GetHandle());

			// Verify queue family support
			{
				if (!indices.IsComplete())
				{
					continue;
				}
			}

			// Verify required device extensions are available
			{
				uint32_t availableDeviceExtensionCount;
				vkEnumerateDeviceExtensionProperties(physicalDevice, nullptr, &availableDeviceExtensionCount, nullptr);
				std::vector<VkExtensionProperties> availableDeviceExtensions(availableDeviceExtensionCount);
				vkEnumerateDeviceExtensionProperties(physicalDevice, nullptr, &availableDeviceExtensionCount, availableDeviceExtensions.data());

				std::unordered_set<std::string> availableDeviceExtensionSet;
				for (const VkExtensionProperties& availableDeviceExtension : availableDeviceExtensions) {
					availableDeviceExtensionSet.emplace(availableDeviceExtension.extensionName);
				}

				bool bRequiredExtensionsAvailable = true;
				for (const char* requiredDeviceExtension : requirements.requiredDeviceExtensions)
				{
					if (availableDeviceExtensionSet.find(requiredDeviceExtension) == availableDeviceExtensionSet.end())
					{
						bRequiredExtensionsAvailable = false;
						break;
					}
				}

				if (!bRequiredExtensionsAvailable)
				{
					continue;
				}
			}

			// Verify swapchain support
			{
				SwapchainSupportDetails swapchainSupport = QuerySwapchainSupport(physicalDevice, requirements.surface);
				if (swapchainSupport.formats.empty() || swapchainSupport.presentModes.empty())
				{
					continue;
				}
			}

			validDevices.push_back(physicalDevice);
		}

		// Select the most fit device. At the moment, we just prefer discrete GPUs.
		VkPhysicalDevice result = VK_NULL_HANDLE;
		for (const VkPhysicalDevice& physicalDevice : validDevices) {
			VulkanQueueFamilyIndices indices = FindQueueFamilies(physicalDevice, requirements.surface->GetHandle());

			VkPhysicalDeviceProperties deviceProperties;
			vkGetPhysicalDeviceProperties(physicalDevice, &deviceProperties);
			VkPhysicalDeviceFeatures deviceFeatures;
			vkGetPhysicalDeviceFeatures(physicalDevice, &deviceFeatures);

			if (deviceProperties.deviceType == VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU)
			{
				LOG_INFO("Located discrete GPU. Selecting device " << deviceProperties.deviceName << ".");
				result = physicalDevice;
				break;
			}
		}

		// If there is no discrete GPU, fallback to the first valid GPU
		if (result == VK_NULL_HANDLE && validDevices.size() != 0) {
			VkPhysicalDeviceProperties deviceProperties;
			vkGetPhysicalDeviceProperties(validDevices[0], &deviceProperties);

			LOG_WARNING("No discrete GPU available, falling back to " << deviceProperties.deviceName << ".")

			result = devices[0];
		}

		ASSERT(result != VK_NULL_HANDLE, "Failed to find a suitable physical device.");

		return result;
	}

	SwapchainSupportDetails QuerySwapchainSupport(VkPhysicalDevice physicalDevice, VulkanSurface* surface)
	{
		SwapchainSupportDetails details;
		vkGetPhysicalDeviceSurfaceCapabilitiesKHR(physicalDevice, surface->GetHandle(), &details.capabilities);

		uint32_t formatCount;
		vkGetPhysicalDeviceSurfaceFormatsKHR(physicalDevice, surface->GetHandle(), &formatCount, nullptr);	
		if (formatCount != 0) {
			details.formats.resize(formatCount);
			vkGetPhysicalDeviceSurfaceFormatsKHR(physicalDevice, surface->GetHandle(), &formatCount, details.formats.data());
		}

		uint32_t presentModeCount;
		vkGetPhysicalDeviceSurfacePresentModesKHR(physicalDevice, surface->GetHandle(), &presentModeCount, nullptr);	
		if (presentModeCount != 0) {
			details.presentModes.resize(presentModeCount);
			vkGetPhysicalDeviceSurfacePresentModesKHR(physicalDevice, surface->GetHandle(), &presentModeCount, details.presentModes.data());
		}
		
		return details;
	}

	VkSurfaceFormatKHR VulkanPhysicalDevice::ChooseSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR>& availableFormats)
	{
		for (const VkSurfaceFormatKHR& availableFormat : availableFormats)
		{
			if (availableFormat.format == VK_FORMAT_B8G8R8A8_SRGB && availableFormat.colorSpace == VK_COLOR_SPACE_SRGB_NONLINEAR_KHR)
			{
				return availableFormat;
			}
		}

		return availableFormats[0];
	}

	VkPresentModeKHR VulkanPhysicalDevice::ChooseSwapPresentMode(const std::vector<VkPresentModeKHR>& availablePresentModes)
	{
		return VK_PRESENT_MODE_FIFO_KHR;
	}

	VkExtent2D VulkanPhysicalDevice::ChooseSwapExtent(const VkSurfaceCapabilitiesKHR& capabilities, VulkanSurface* surface)
	{
		if (capabilities.currentExtent.width != std::numeric_limits<uint32_t>::max())
		{
			return capabilities.currentExtent;
		} 
		else 
		{
			int width, height;
			surface->GetWindow()->GetFramebufferSize(&width, &height);
			
			VkExtent2D actualExtent = {
				static_cast<uint32_t>(width),
				static_cast<uint32_t>(height)
			};
			
			actualExtent.width = std::clamp(actualExtent.width, capabilities.minImageExtent.width, capabilities.maxImageExtent.width);
			actualExtent.height = std::clamp(actualExtent.height, capabilities.minImageExtent.height, capabilities.maxImageExtent.height);
			
			return actualExtent;
		}
	}
}
