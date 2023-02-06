#pragma once

#include "renderer_core.h"
#include "vulkan/vulkan_physical_device.h"

namespace glibby
{
	struct VulkanInstanceRequirements
	{
		std::vector<const char*> requiredBaseExtensions;
		std::vector<const char*> requiredLayers;
	};

	class VulkanInstance
	{
	public:
		VulkanInstance(const VulkanInstanceRequirements& instanceRequirements);
		~VulkanInstance();

		// TODO: destroy copy/ass constructor

		VkInstance GetHandle() { return handle; }

		void SetupDebugMessenger();
		VulkanPhysicalDevice PickPhysicalDevice(const VulkanPhysicalDeviceRequirements& requirements) const;

	private:
		VkInstance handle = VK_NULL_HANDLE;
#ifdef GLIBBY_DEBUG
		VkDebugUtilsMessengerEXT debugMessenger = VK_NULL_HANDLE;
#endif
	};
}