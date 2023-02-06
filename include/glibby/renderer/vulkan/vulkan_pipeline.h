#pragma once

#include "renderer_core.h"

#include <vector>

namespace glibby
{
	class VulkanDevice;
	class VulkanRenderPass;

	class VulkanPipeline
	{
	public:
		VulkanPipeline(VulkanDevice* device, VulkanRenderPass* renderPass, VkDescriptorSetLayout descriptorSetLayout);
		~VulkanPipeline();

		VkPipelineLayout GetPipelineLayout() { return pipelineLayout; }
		VkPipeline GetPipeline() { return graphicsPipeline; }

		VkPipeline GetHandle() { return handle; }
	private:
		void CreateGraphicsPipeline(VulkanRenderPass* renderPass, VkDescriptorSetLayout descriptorSetLayout);
		VkShaderModule CreateShaderModule(const std::vector<char>& code);
	private:
		VulkanDevice* device;
		VkPipeline handle;

		VkPipelineLayout pipelineLayout;
		VkPipeline graphicsPipeline;
	};
}