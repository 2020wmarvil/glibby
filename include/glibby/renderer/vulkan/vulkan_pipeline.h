#pragma once

#include "renderer_core.h"
#include "vulkan/vulkan_material.h"

#include <vector>

namespace glibby
{
	class VulkanDevice;
	class VulkanRenderPass;

	class VulkanPipeline
	{
	public:
		VulkanPipeline(VulkanDevice* device, VulkanRenderPass* renderPass, const PipelineInfo& pipelineInfo, VkDescriptorSetLayout descriptorSetLayout);
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

		PipelineInfo pipelineInfo;

		VkPipelineLayout pipelineLayout;
		VkPipeline graphicsPipeline;
	};
}