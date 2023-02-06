#pragma once

#include "renderer_core.h"

namespace glibby
{
	class VulkanDevice;

	class VulkanRenderPass
	{
	public:
		VulkanRenderPass(VulkanDevice* device, VkFormat imageFormat);
		~VulkanRenderPass();

		VkRenderPass GetHandle() { return handle; }

	private:
		VulkanDevice* device;
		VkRenderPass handle;
	};
}