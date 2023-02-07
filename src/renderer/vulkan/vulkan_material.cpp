#include "vulkan/vulkan_material.h"

#include "texture.h"
#include "vulkan/vulkan_device.h"
#include "vulkan/vulkan_pipeline.h"
#include "vulkan/vulkan_texture.h"
#include "vulkan/vulkan_image.h"
#include "camera.h"

#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

#include <array>
#include <chrono>

namespace glibby
{
	struct UniformBufferObject
	{
		glm::mat4 model;
		glm::mat4 view;
		glm::mat4 proj;
	};

	VulkanMaterial::VulkanMaterial(VulkanDevice* device, VulkanRenderPass* renderPass)
	{
		this->device = device;

		int texWidth, texHeight, mipLevels;
		void* data = LoadTextureData("../../resources/textures/viking_room.png", &texWidth, &texHeight, &mipLevels);
		texture = std::make_unique<VulkanTexture>(device, data, texWidth, texHeight, mipLevels);

		FreeTextureData(data);

		CreateUniformBuffers();
		CreateDescriptorSetLayout();
		CreateDescriptorSets();

		pipeline = std::make_unique<VulkanPipeline>(device, renderPass, descriptorSetLayout);
	}

	VulkanMaterial::~VulkanMaterial()
	{
		vkDestroyDescriptorSetLayout(device->GetHandle(), descriptorSetLayout, nullptr);

		for (size_t i = 0; i < MAX_FRAMES_IN_FLIGHT; i++)
		{
			vkDestroyBuffer(device->GetHandle(), uniformBuffers[i], nullptr);
			vkFreeMemory(device->GetHandle(), uniformBuffersMemory[i], nullptr);
		}
	}

	void VulkanMaterial::UpdateUniformBuffer(uint32_t currentImage, uint32_t width, uint32_t height, Camera* camera)
	{
		static auto startTime = std::chrono::high_resolution_clock::now();

		auto currentTime = std::chrono::high_resolution_clock::now();
		float time = std::chrono::duration<float, std::chrono::seconds::period>(currentTime - startTime).count();

		UniformBufferObject ubo{};
		ubo.model = glm::mat4(1.0f);
		ubo.view = camera->GetView();
		ubo.proj = camera->GetPerspective(width, height);
		ubo.proj[1][1] *= -1;

		memcpy(uniformBuffersMapped[currentImage], &ubo, sizeof(ubo));
	}

	VkPipelineLayout VulkanMaterial::GetPipelineLayout()
	{
		return pipeline->GetPipelineLayout();
	}

	VkPipeline VulkanMaterial::GetPipeline()
	{
		return pipeline->GetPipeline();
	}

	void VulkanMaterial::CreateUniformBuffers()
	{
		VkDeviceSize bufferSize = sizeof(UniformBufferObject);
		uniformBuffers.resize(MAX_FRAMES_IN_FLIGHT);
		uniformBuffersMemory.resize(MAX_FRAMES_IN_FLIGHT);
		uniformBuffersMapped.resize(MAX_FRAMES_IN_FLIGHT);

		for (size_t i = 0; i < MAX_FRAMES_IN_FLIGHT; i++)
		{
			device->CreateBuffer(bufferSize, VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, uniformBuffers[i], uniformBuffersMemory[i]);
			VK_CHECK(vkMapMemory(device->GetHandle(), uniformBuffersMemory[i], 0, bufferSize, 0, &uniformBuffersMapped[i]), "Failed to map uniform buffer.");
		}
	}

	void VulkanMaterial::CreateDescriptorSetLayout()
	{
		VkDescriptorSetLayoutBinding uboLayoutBinding{};
		uboLayoutBinding.binding = 0;
		uboLayoutBinding.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
		uboLayoutBinding.descriptorCount = 1;
		uboLayoutBinding.stageFlags = VK_SHADER_STAGE_VERTEX_BIT;
		uboLayoutBinding.pImmutableSamplers = nullptr;

		VkDescriptorSetLayoutBinding samplerLayoutBinding{};
		samplerLayoutBinding.binding = 1;
		samplerLayoutBinding.descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
		samplerLayoutBinding.descriptorCount = 1;
		samplerLayoutBinding.stageFlags = VK_SHADER_STAGE_FRAGMENT_BIT;
		samplerLayoutBinding.pImmutableSamplers = nullptr;

		std::array<VkDescriptorSetLayoutBinding, 2> bindings = { uboLayoutBinding, samplerLayoutBinding };
		VkDescriptorSetLayoutCreateInfo layoutInfo = { VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO };
		layoutInfo.bindingCount = static_cast<uint32_t>(bindings.size());
		layoutInfo.pBindings = bindings.data();

		VK_CHECK(vkCreateDescriptorSetLayout(device->GetHandle(), &layoutInfo, nullptr, &descriptorSetLayout), "Failed to create descriptor layout.");
	}

	void VulkanMaterial::CreateDescriptorSets()
	{
		std::vector<VkDescriptorSetLayout> layouts(MAX_FRAMES_IN_FLIGHT, descriptorSetLayout);
		VkDescriptorSetAllocateInfo allocInfo = { VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO };
		allocInfo.descriptorPool = device->descriptorPool;
		allocInfo.descriptorSetCount = static_cast<uint32_t>(MAX_FRAMES_IN_FLIGHT);
		allocInfo.pSetLayouts = layouts.data();

		descriptorSets.resize(MAX_FRAMES_IN_FLIGHT);
		VK_CHECK(vkAllocateDescriptorSets(device->GetHandle(), &allocInfo, descriptorSets.data()), "Failed to allocate descriptor sets.");

		for (size_t i = 0; i < MAX_FRAMES_IN_FLIGHT; i++) {
			VkDescriptorBufferInfo bufferInfo{};
			bufferInfo.buffer = uniformBuffers[i];
			bufferInfo.offset = 0;
			bufferInfo.range = sizeof(UniformBufferObject);

			VkDescriptorImageInfo imageInfo{};
			imageInfo.imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
			imageInfo.imageView = texture->GetImage()->GetImageView();
			imageInfo.sampler = texture->GetSampler();

			std::array<VkWriteDescriptorSet, 2> descriptorWrites{};
			descriptorWrites[0].sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET; // TODO: instancing: https://www.reddit.com/r/vulkan/comments/4ibeoh/how_to_set_per_objects_uniform/
			descriptorWrites[0].dstSet = descriptorSets[i];
			descriptorWrites[0].dstBinding = 0;
			descriptorWrites[0].dstArrayElement = 0;
			descriptorWrites[0].descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
			descriptorWrites[0].descriptorCount = 1;
			descriptorWrites[0].pBufferInfo = &bufferInfo;

			descriptorWrites[1].sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
			descriptorWrites[1].dstSet = descriptorSets[i];
			descriptorWrites[1].dstBinding = 1;
			descriptorWrites[1].dstArrayElement = 0;
			descriptorWrites[1].descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
			descriptorWrites[1].descriptorCount = 1;
			descriptorWrites[1].pImageInfo = &imageInfo;

			vkUpdateDescriptorSets(device->GetHandle(), static_cast<uint32_t>(descriptorWrites.size()), descriptorWrites.data(), 0, nullptr);
		}
	}
}
