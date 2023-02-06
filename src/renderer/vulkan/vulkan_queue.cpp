#include "vulkan/vulkan_queue.h"

#include "vulkan/vulkan_physical_device.h"

namespace glibby
{
	VulkanQueueFamilyIndices FindQueueFamilies(VkPhysicalDevice physicalDevice, VkSurfaceKHR surface)
	{
		VulkanQueueFamilyIndices indices;

		uint32_t queueFamilyCount = 0;
		vkGetPhysicalDeviceQueueFamilyProperties(physicalDevice, &queueFamilyCount, nullptr);
		std::vector<VkQueueFamilyProperties> queueFamilies(queueFamilyCount);
		vkGetPhysicalDeviceQueueFamilyProperties(physicalDevice, &queueFamilyCount, queueFamilies.data());

		// Grab first valid graphics and present families
		for (int i = 0; i < queueFamilyCount; i++)
		{
			if (queueFamilies[i].queueFlags & VK_QUEUE_GRAPHICS_BIT)
			{
				indices.graphicsFamily = i;
			}

			VkBool32 bPresentSupport = false;
			vkGetPhysicalDeviceSurfaceSupportKHR(physicalDevice, i, surface, &bPresentSupport);
			if (bPresentSupport)
			{
				indices.presentFamily = i;
			}

			if (indices.graphicsFamily.has_value() && indices.presentFamily.has_value())
			{
				break;
			}
		}

		// Try to grab a compute family that is distinct from graphics family
		for (int i = 0; i < queueFamilyCount; i++)
		{
			if (queueFamilies[i].queueFlags & VK_QUEUE_COMPUTE_BIT)
			{
				indices.computeFamily = i;

				if (indices.graphicsFamily.has_value() && indices.graphicsFamily.value() != i)
				{
					break;
				}
			}
		}

		// Try to grab a transfer family that is distinct from graphics and compute
		for (int i = 0; i < queueFamilyCount; i++)
		{
			if (queueFamilies[i].queueFlags & VK_QUEUE_TRANSFER_BIT)
			{
				indices.transferFamily = i;

				// Stop searching if we find a transfer family that is distinct from graphics and compute
				if (indices.graphicsFamily.has_value() && indices.graphicsFamily.value() != i
					&& indices.computeFamily.has_value() && indices.computeFamily.value() != i)
				{
					break;
				}
			}
		}

		return indices;
	}
}
