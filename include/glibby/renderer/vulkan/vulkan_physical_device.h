#pragma once

#include "renderer_core.h"
#include "vulkan/vulkan_queue.h"

#include <vector>

namespace glibby
{
	class VulkanInstance;
	class VulkanSurface;
	struct VulkanQueueFamilyIndices;

	// TODO: make requirements more useful
	struct VulkanPhysicalDeviceRequirements
	{
		std::vector<const char*> requiredDeviceExtensions;
		VulkanSurface* surface;
	};

	struct SwapchainSupportDetails
	{
		VkSurfaceCapabilitiesKHR capabilities;
		std::vector<VkSurfaceFormatKHR> formats;
		std::vector<VkPresentModeKHR> presentModes;
	};

	SwapchainSupportDetails QuerySwapchainSupport(VkPhysicalDevice physicalDevice, VulkanSurface* surface);

	class VulkanPhysicalDevice
	{
	public:
		VulkanPhysicalDevice(VulkanInstance* instance, const VulkanPhysicalDeviceRequirements& physicalDeviceRequirements);

		VkPhysicalDevice GetHandle() { return handle; }

		VkSurfaceFormatKHR ChooseSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR>& availableFormats);
		VkPresentModeKHR ChooseSwapPresentMode(const std::vector<VkPresentModeKHR>& availablePresentModes);
		VkExtent2D ChooseSwapExtent(const VkSurfaceCapabilitiesKHR& capabilities, VulkanSurface* surface);

	private:
		VkPhysicalDevice PickPhysicalDevice(const VulkanPhysicalDeviceRequirements& requirements) const;
		
	public:
		VulkanInstance* instance;
		VkPhysicalDeviceProperties properties;
		VkPhysicalDeviceFeatures features;
		VulkanQueueFamilyIndices queueFamilyIndices;
	private:
		VkPhysicalDevice handle;
	};
}