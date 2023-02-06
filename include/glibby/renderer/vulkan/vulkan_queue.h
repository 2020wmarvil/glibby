#pragma once

#include "renderer_core.h"

#include <optional>
#include <vector>

namespace glibby
{
	struct VulkanQueueFamilyIndices
	{
		std::optional<uint32_t> graphicsFamily;
		std::optional<uint32_t> presentFamily;
		std::optional<uint32_t> computeFamily;
		std::optional<uint32_t> transferFamily;

		bool IsComplete()
		{
			return graphicsFamily.has_value() && presentFamily.has_value() && computeFamily.has_value() && transferFamily.has_value();
		}
	};
	
	VulkanQueueFamilyIndices FindQueueFamilies(VkPhysicalDevice device, VkSurfaceKHR surface);

	class VulkanQueue
	{
	public:
		VkQueue queue;
	};
}