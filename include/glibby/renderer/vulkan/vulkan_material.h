#pragma once

#include "renderer_core.h"
#include "material.h"

#include <vector>

namespace glibby
{
	class VulkanDevice;
	class VulkanRenderPass;
	class VulkanPipeline;
	class VulkanTexture;
	class Camera;

	class VulkanMaterial : public Material
	{
	public:
		VulkanMaterial(VulkanDevice* device, VulkanRenderPass* renderPass);
		~VulkanMaterial();

		void UpdateUniformBuffer(uint32_t currentImage, uint32_t width, uint32_t height, Camera* camera);

		VkPipelineLayout GetPipelineLayout();
		VkPipeline GetPipeline();
		VkDescriptorSet GetDescriptorSet(uint32_t currentFrame) { return descriptorSets[currentFrame]; }

	private:
		void CreateUniformBuffers();
		void CreateDescriptorSetLayout();
		void CreateDescriptorSets();

	private:
		VulkanDevice* device;

		std::unique_ptr<VulkanPipeline> pipeline;
		std::unique_ptr<VulkanTexture> texture;

		std::vector<VkBuffer> uniformBuffers;
		std::vector<VkDeviceMemory> uniformBuffersMemory;
		std::vector<void*> uniformBuffersMapped;

		VkDescriptorSetLayout descriptorSetLayout;
		std::vector<VkDescriptorSet> descriptorSets;
	};
}
