#include "vulkan_engine/VulkanInitializers.h"

namespace vkinit
{
	VkCommandPoolCreateInfo command_pool_create_info(uint32_t queueFamilyIndex, VkCommandPoolCreateFlags flags /*= 0*/)
	{
		VkCommandPoolCreateInfo info = {};
		info.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
		info.pNext = nullptr;

		info.queueFamilyIndex = queueFamilyIndex;
		info.flags = flags;
		return info;
	}

	VkCommandBufferAllocateInfo command_buffer_allocate_info(VkCommandPool pool, uint32_t count /*= 1*/, VkCommandBufferLevel level /*= VK_COMMAND_BUFFER_LEVEL_PRIMARY*/)
	{
		VkCommandBufferAllocateInfo info = {};
		info.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
		info.pNext = nullptr;

		info.commandPool = pool;
		info.commandBufferCount = count;
		info.level = level;
		return info;
	}

	VkFenceCreateInfo fence_create_info(VkFenceCreateFlags flags)
	{
		VkFenceCreateInfo fenceCreateInfo = { VK_STRUCTURE_TYPE_FENCE_CREATE_INFO };
		fenceCreateInfo.flags = flags;
		return fenceCreateInfo;
	}

	VkSemaphoreCreateInfo semaphore_create_info(VkSemaphoreCreateFlags flags)
	{
		VkSemaphoreCreateInfo semCreateInfo = { VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO };
		semCreateInfo.flags = flags;
		return semCreateInfo;
	}

	VkPipelineShaderStageCreateInfo pipeline_shader_stage_create_info(VkShaderStageFlagBits stage, VkShaderModule shaderModule)
	{
		VkPipelineShaderStageCreateInfo info = { VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO };
		info.stage = stage;
		info.module = shaderModule;
		info.pName = "main";
		return info;
	}

	VkPipelineVertexInputStateCreateInfo vertex_input_state_create_info()
	{
		VkPipelineVertexInputStateCreateInfo info = { VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO };
		info.vertexBindingDescriptionCount = 0;
		info.vertexAttributeDescriptionCount = 0;
		return info;
	}

	VkPipelineInputAssemblyStateCreateInfo input_assembly_create_info(VkPrimitiveTopology topology)
	{
		VkPipelineInputAssemblyStateCreateInfo info = { VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO };
		info.topology = topology;
		info.primitiveRestartEnable = VK_FALSE;
		return info;
	}

	VkPipelineRasterizationStateCreateInfo rasterization_state_create_info(VkPolygonMode polygonMode)
	{
		VkPipelineRasterizationStateCreateInfo info = { VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO };
		info.depthClampEnable = VK_FALSE;
		info.rasterizerDiscardEnable = VK_FALSE;
		info.polygonMode = polygonMode;
		info.lineWidth = 1.0f;
		info.cullMode = VK_CULL_MODE_NONE;
		info.frontFace = VK_FRONT_FACE_CLOCKWISE;
		info.depthBiasEnable = VK_FALSE;
		info.depthBiasConstantFactor = 0.0f;
		info.depthBiasClamp = 0.0f;
		info.depthBiasSlopeFactor = 0.0f;
		return info;
	}

	VkPipelineMultisampleStateCreateInfo multisampling_state_create_info()
	{
		VkPipelineMultisampleStateCreateInfo info = { VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO };
		info.sampleShadingEnable = VK_FALSE;
		info.rasterizationSamples = VK_SAMPLE_COUNT_1_BIT;
		info.minSampleShading = 1.0f;
		info.pSampleMask = nullptr;
		info.alphaToCoverageEnable = VK_FALSE;
		info.alphaToOneEnable = VK_FALSE;
		return info;
	}

	VkPipelineDepthStencilStateCreateInfo depth_stencil_create_info(bool bDepthTest, bool bDepthWrite, VkCompareOp compareOp)
	{
		VkPipelineDepthStencilStateCreateInfo info = { VK_STRUCTURE_TYPE_PIPELINE_DEPTH_STENCIL_STATE_CREATE_INFO };
		info.depthTestEnable = bDepthTest ? VK_TRUE : VK_FALSE;
		info.depthWriteEnable = bDepthWrite ? VK_TRUE : VK_FALSE;
		info.depthCompareOp = bDepthTest ? compareOp : VK_COMPARE_OP_ALWAYS;
		info.depthBoundsTestEnable = VK_FALSE;
		info.minDepthBounds = 0.0f; // Optional
		info.maxDepthBounds = 1.0f; // Optional
		info.stencilTestEnable = VK_FALSE;
		return info;
	}

	VkPipelineColorBlendAttachmentState color_blend_attachment_state()
	{
		VkPipelineColorBlendAttachmentState colorBlendAttachment = {};
		colorBlendAttachment.colorWriteMask =
			VK_COLOR_COMPONENT_R_BIT
			| VK_COLOR_COMPONENT_G_BIT
			| VK_COLOR_COMPONENT_B_BIT
			| VK_COLOR_COMPONENT_A_BIT;
		colorBlendAttachment.blendEnable = VK_FALSE;
		return colorBlendAttachment;
	}

	VkPipelineLayoutCreateInfo pipeline_layout_create_info()
	{
		VkPipelineLayoutCreateInfo info = { VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO };
		info.flags = 0;
		info.setLayoutCount = 0;
		info.pSetLayouts = nullptr;
		info.pushConstantRangeCount = 0;
		info.pPushConstantRanges = nullptr;
		return info;
	}

	VkImageCreateInfo image_create_info(VkFormat format, VkImageUsageFlags usageFlags, VkExtent3D extent)
	{
		VkImageCreateInfo info = { VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO };
		info.imageType = VK_IMAGE_TYPE_2D;
		info.format = format;
		info.extent = extent;
		info.mipLevels = 1;
		info.arrayLayers = 1;
		info.samples = VK_SAMPLE_COUNT_1_BIT;
		info.tiling = VK_IMAGE_TILING_OPTIMAL;
		info.usage = usageFlags;
		return info;
	}

	VkImageViewCreateInfo imageview_create_info(VkFormat format, VkImage image, VkImageAspectFlags aspectFlags)
	{
		VkImageViewCreateInfo info = { VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO };
		info.viewType = VK_IMAGE_VIEW_TYPE_2D;
		info.image = image;
		info.format = format;
		info.subresourceRange.baseMipLevel = 0;
		info.subresourceRange.levelCount = 1;
		info.subresourceRange.baseArrayLayer = 0;
		info.subresourceRange.layerCount = 1;
		info.subresourceRange.aspectMask = aspectFlags;
		return info;
	}

	VkDescriptorSetLayoutBinding descriptorset_layout_binding(VkDescriptorType type, VkShaderStageFlags stageFlags, uint32_t binding)
	{
		VkDescriptorSetLayoutBinding setbind = {};
		setbind.binding = binding;
		setbind.descriptorCount = 1;
		setbind.descriptorType = type;
		setbind.pImmutableSamplers = nullptr;
		setbind.stageFlags = stageFlags;
		return setbind;
	}

	VkWriteDescriptorSet write_descriptor_buffer(VkDescriptorType type, VkDescriptorSet dstSet, VkDescriptorBufferInfo* bufferInfo, uint32_t binding)
	{
		VkWriteDescriptorSet write = { VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET };
		write.dstBinding = binding;
		write.dstSet = dstSet;
		write.descriptorCount = 1;
		write.descriptorType = type;
		write.pBufferInfo = bufferInfo;
		return write;
	}

	VkCommandBufferBeginInfo command_buffer_begin_info(VkCommandBufferUsageFlags flags /*= 0*/)
	{
		VkCommandBufferBeginInfo info = { VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO };
		info.pInheritanceInfo = nullptr;
		info.flags = flags;
		return info;
	}

	VkSubmitInfo submit_info(VkCommandBuffer* cmd)
	{
		VkSubmitInfo info = { VK_STRUCTURE_TYPE_SUBMIT_INFO };
		info.waitSemaphoreCount = 0;
		info.pWaitSemaphores = nullptr;
		info.pWaitDstStageMask = nullptr;
		info.commandBufferCount = 1;
		info.pCommandBuffers = cmd;
		info.signalSemaphoreCount = 0;
		info.pSignalSemaphores = nullptr;
		return info;
	}

	VkSamplerCreateInfo sampler_create_info(VkFilter filters, VkSamplerAddressMode samplerAddressMode /*= VK_SAMPLER_ADDRESS_MODE_REPEAT*/)
	{
		VkSamplerCreateInfo info = { VK_STRUCTURE_TYPE_SAMPLER_CREATE_INFO };
		info.magFilter = filters;
		info.minFilter = filters;
		info.addressModeU = samplerAddressMode;
		info.addressModeV = samplerAddressMode;
		info.addressModeW = samplerAddressMode;
		return info;
	}

	VkWriteDescriptorSet write_descriptor_image(VkDescriptorType type, VkDescriptorSet dstSet, VkDescriptorImageInfo* imageInfo, uint32_t binding)
	{
		VkWriteDescriptorSet write = { VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET };
		write.dstBinding = binding;
		write.dstSet = dstSet;
		write.descriptorCount = 1;
		write.descriptorType = type;
		write.pImageInfo = imageInfo;
		return write;
	}
}