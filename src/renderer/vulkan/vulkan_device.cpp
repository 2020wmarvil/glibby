#include "vulkan/vulkan_device.h"

#include "renderer_core.h"
#include "vulkan/vulkan_instance.h"
#include "vulkan/vulkan_physical_device.h"
#include "vulkan/vulkan_swapchain.h"
#include "vulkan/vulkan_model.h"
#include "vulkan/vulkan_material.h"
#include "vulkan/vulkan_render_pass.h"
#include "vulkan/vulkan_renderer.h"

#include <vector>
#include <set>
#include <array>

namespace glibby
{
	VulkanDevice::VulkanDevice(VulkanRenderer* renderer, VulkanPhysicalDevice* physicalDevice, const VulkanDeviceRequirements& deviceRequirements)
		: physicalDevice(physicalDevice), renderer(renderer)
	{
		std::vector<VkDeviceQueueCreateInfo> queueCreateInfos;
		std::set<uint32_t> uniqueQueueFamilies =
		{
			physicalDevice->queueFamilyIndices.graphicsFamily.value(),
			physicalDevice->queueFamilyIndices.presentFamily.value(),
			physicalDevice->queueFamilyIndices.transferFamily.value(),
			physicalDevice->queueFamilyIndices.computeFamily.value()
		};
		
		float queuePriority = 1.0f;
		for (uint32_t queueFamily : uniqueQueueFamilies) {
			VkDeviceQueueCreateInfo queueCreateInfo = { VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO };
			queueCreateInfo.queueFamilyIndex = queueFamily;
			queueCreateInfo.queueCount = 1;
			queueCreateInfo.pQueuePriorities = &queuePriority;
			queueCreateInfos.push_back(queueCreateInfo);
		}

		VkDeviceCreateInfo deviceCreateInfo = { VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO };
		deviceCreateInfo.queueCreateInfoCount = static_cast<uint32_t>(queueCreateInfos.size());
		deviceCreateInfo.pQueueCreateInfos = queueCreateInfos.data();

		VkPhysicalDeviceFeatures deviceFeatures{};
		deviceFeatures.samplerAnisotropy = physicalDevice->features.samplerAnisotropy ? VK_TRUE : VK_FALSE;
		deviceFeatures.sampleRateShading = VK_TRUE;
		deviceCreateInfo.pEnabledFeatures = &deviceFeatures;

		deviceCreateInfo.enabledExtensionCount = static_cast<uint32_t>(deviceRequirements.requiredDeviceExtensions.size());
		deviceCreateInfo.ppEnabledExtensionNames = deviceRequirements.requiredDeviceExtensions.data();

		deviceCreateInfo.pNext = nullptr;

#ifdef GLIBBY_DEBUG
		deviceCreateInfo.enabledLayerCount = static_cast<uint32_t>(deviceRequirements.requiredDeviceLayers.size());
		deviceCreateInfo.ppEnabledLayerNames = deviceRequirements.requiredDeviceLayers.data();
#else
		deviceCreateInfo.enabledLayerCount = 0;
		deviceCreateInfo.ppEnabledLayerNames = nullptr;
#endif

		instance = physicalDevice->instance;

		VK_CHECK(vkCreateDevice(physicalDevice->GetHandle(), &deviceCreateInfo, nullptr, &handle), "Failed to create logical device.");

		vkGetDeviceQueue(handle, physicalDevice->queueFamilyIndices.graphicsFamily.value(), 0, &graphicsQueue.queue);
		vkGetDeviceQueue(handle, physicalDevice->queueFamilyIndices.presentFamily.value(), 0, &presentQueue.queue);
		vkGetDeviceQueue(handle, physicalDevice->queueFamilyIndices.computeFamily.value(), 0, &computeQueue.queue);
		vkGetDeviceQueue(handle, physicalDevice->queueFamilyIndices.transferFamily.value(), 0, &transferQueue.queue);

		CreateCommandPools();
		CreateCommandBuffers();
		CreateDescriptorPools();
		CreateSyncObjects();
	}

	VulkanDevice::~VulkanDevice()
	{
		for (size_t i = 0; i < MAX_FRAMES_IN_FLIGHT; i++)
		{
			vkDestroySemaphore(handle, imageAvailableSemaphores[i], nullptr);
			vkDestroySemaphore(handle, renderFinishedSemaphores[i], nullptr);
			vkDestroyFence(handle, inFlightFences[i], nullptr);
		}

		vkDestroyCommandPool(handle, graphicsCommandPool, nullptr);
		vkDestroyCommandPool(handle, transferCommandPool, nullptr);
		vkDestroyDescriptorPool(handle, descriptorPool, nullptr);
		vkDestroyCommandPool(handle, computeCommandPool, nullptr);

		vkDestroyDevice(handle, nullptr);
	}

	void VulkanDevice::DrawFrame(VulkanModel* model, VulkanMaterial* material, bool framebufferResized)
	{
		vkWaitForFences(handle, 1, &inFlightFences[currentFrame], VK_TRUE, UINT64_MAX);

		uint32_t imageIndex;
		VkResult acquireImageResult = vkAcquireNextImageKHR(handle, swapchain->GetHandle(), UINT64_MAX, imageAvailableSemaphores[currentFrame], VK_NULL_HANDLE, &imageIndex);
		if (acquireImageResult == VK_ERROR_OUT_OF_DATE_KHR)
		{
			swapchain->RecreateSwapchain();
			return;
		}
		else if (acquireImageResult != VK_SUCCESS && acquireImageResult != VK_SUBOPTIMAL_KHR)
		{
			LOG_ERROR("Failed to acquire swapchain image.");
		}

		// Delay fence reset until we are sure we are submitting work, to avoid deadlock from early exit
		vkResetFences(handle, 1, &inFlightFences[currentFrame]);

		vkResetCommandBuffer(graphicsCommandBuffers[currentFrame], 0);
		RecordGraphicsCommandBuffer(graphicsCommandBuffers[currentFrame], imageIndex, model, material);

		uint32_t width, height;
		swapchain->GetSize(&width, &height);
		material->UpdateUniformBuffer(currentFrame, width, height, renderer->GetCamera());

		VkSubmitInfo submitInfo = { VK_STRUCTURE_TYPE_SUBMIT_INFO };

		VkSemaphore waitSemaphores[] = { imageAvailableSemaphores[currentFrame] };
		VkPipelineStageFlags waitStages[] = { VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT };
		submitInfo.waitSemaphoreCount = 1;
		submitInfo.pWaitSemaphores = waitSemaphores;
		submitInfo.pWaitDstStageMask = waitStages;
		submitInfo.commandBufferCount = 1;
		submitInfo.pCommandBuffers = &graphicsCommandBuffers[currentFrame];

		VkSemaphore signalSemaphores[] = { renderFinishedSemaphores[currentFrame] };
		submitInfo.signalSemaphoreCount = 1;
		submitInfo.pSignalSemaphores = signalSemaphores;

		VK_CHECK(vkQueueSubmit(graphicsQueue.queue, 1, &submitInfo, inFlightFences[currentFrame]), "Failed to submit draw command buffer.");

		VkPresentInfoKHR presentInfo = { VK_STRUCTURE_TYPE_PRESENT_INFO_KHR };
		presentInfo.waitSemaphoreCount = 1;
		presentInfo.pWaitSemaphores = signalSemaphores;

		VkSwapchainKHR swapchains[] = { swapchain->GetHandle() };
		presentInfo.swapchainCount = 1;
		presentInfo.pSwapchains = swapchains;
		presentInfo.pImageIndices = &imageIndex;
		presentInfo.pResults = nullptr;

		VkResult presentResult = vkQueuePresentKHR(presentQueue.queue, &presentInfo);
		if (presentResult == VK_ERROR_OUT_OF_DATE_KHR || presentResult == VK_SUBOPTIMAL_KHR || framebufferResized)
		{
			framebufferResized = false;
			swapchain->RecreateSwapchain();
		}
		else if (presentResult != VK_SUCCESS)
		{
			LOG_ERROR("Failed to present swapchain image.");
		}

		currentFrame = (currentFrame + 1) % MAX_FRAMES_IN_FLIGHT;
	}

	VkSampleCountFlagBits VulkanDevice::GetMaxUsableSampleCount()
	{
		VkSampleCountFlags counts = physicalDevice->properties.limits.framebufferColorSampleCounts & physicalDevice->properties.limits.framebufferDepthSampleCounts;

		if (counts & VK_SAMPLE_COUNT_64_BIT) return VK_SAMPLE_COUNT_64_BIT;
		if (counts & VK_SAMPLE_COUNT_32_BIT) return VK_SAMPLE_COUNT_32_BIT;
		if (counts & VK_SAMPLE_COUNT_16_BIT) return VK_SAMPLE_COUNT_16_BIT;
		if (counts & VK_SAMPLE_COUNT_8_BIT) return VK_SAMPLE_COUNT_8_BIT;
		if (counts & VK_SAMPLE_COUNT_4_BIT) return VK_SAMPLE_COUNT_4_BIT;
		if (counts & VK_SAMPLE_COUNT_2_BIT) return VK_SAMPLE_COUNT_2_BIT;
		
		return VK_SAMPLE_COUNT_1_BIT;
	}

	VkFormat VulkanDevice::FindSupportedFormat(const std::vector<VkFormat>& candidates, VkImageTiling tiling, VkFormatFeatureFlags features)
	{
		for (VkFormat format : candidates)
		{
			VkFormatProperties props;
			vkGetPhysicalDeviceFormatProperties(physicalDevice->GetHandle(), format, &props);

			if (tiling == VK_IMAGE_TILING_LINEAR && (props.linearTilingFeatures & features) == features)
			{
				return format;
			}
			else if (tiling == VK_IMAGE_TILING_OPTIMAL && (props.optimalTilingFeatures & features) == features)
			{
				return format;
			}
		}

		LOG_ERROR("Failed to find a supported format.");

		return VK_FORMAT_UNDEFINED;
	}

	VkFormat VulkanDevice::FindDepthFormat()
	{
		return FindSupportedFormat(
			{ VK_FORMAT_D32_SFLOAT, VK_FORMAT_D32_SFLOAT_S8_UINT, VK_FORMAT_D24_UNORM_S8_UINT },
			VK_IMAGE_TILING_OPTIMAL,
			VK_FORMAT_FEATURE_DEPTH_STENCIL_ATTACHMENT_BIT
		);
	}

	bool VulkanDevice::HasStencilComponent(VkFormat format)
	{
		return format == VK_FORMAT_D32_SFLOAT_S8_UINT || format == VK_FORMAT_D24_UNORM_S8_UINT;
	}

	void VulkanDevice::CreateCommandPools()
	{
		VkCommandPoolCreateInfo graphicsPoolInfo = { VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO };
		graphicsPoolInfo.flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;
		graphicsPoolInfo.queueFamilyIndex = physicalDevice->queueFamilyIndices.graphicsFamily.value();

		VkCommandPoolCreateInfo transferPoolInfo = { VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO };
		transferPoolInfo.flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;
		transferPoolInfo.queueFamilyIndex = physicalDevice->queueFamilyIndices.transferFamily.value();

		VkCommandPoolCreateInfo computePoolInfo = { VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO };
		computePoolInfo.flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;
		computePoolInfo.queueFamilyIndex = physicalDevice->queueFamilyIndices.computeFamily.value();

		VK_CHECK(vkCreateCommandPool(handle, &graphicsPoolInfo, nullptr, &graphicsCommandPool), "Failed to create graphics command pool.");
		VK_CHECK(vkCreateCommandPool(handle, &transferPoolInfo, nullptr, &transferCommandPool), "Failed to create transfer command pool.");
		VK_CHECK(vkCreateCommandPool(handle, &computePoolInfo, nullptr, &computeCommandPool), "Failed to create compute command pool.");
	}

	void VulkanDevice::CreateDescriptorPools()
	{
		std::array<VkDescriptorPoolSize, 2> poolSizes{};
		poolSizes[0].type = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
		poolSizes[0].descriptorCount = static_cast<uint32_t>(MAX_FRAMES_IN_FLIGHT);
		poolSizes[1].type = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
		poolSizes[1].descriptorCount = static_cast<uint32_t>(MAX_FRAMES_IN_FLIGHT);

		VkDescriptorPoolCreateInfo poolInfo = { VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO };
		poolInfo.poolSizeCount = static_cast<uint32_t>(poolSizes.size());
		poolInfo.pPoolSizes = poolSizes.data();
		poolInfo.maxSets = static_cast<uint32_t>(MAX_FRAMES_IN_FLIGHT);
		poolInfo.flags = 0;

		VK_CHECK(vkCreateDescriptorPool(handle, &poolInfo, nullptr, &descriptorPool), "Failed to create descriptor pool.");
	}

	void VulkanDevice::CreateSyncObjects()
	{
		imageAvailableSemaphores.resize(MAX_FRAMES_IN_FLIGHT);
		renderFinishedSemaphores.resize(MAX_FRAMES_IN_FLIGHT);
		inFlightFences.resize(MAX_FRAMES_IN_FLIGHT);

		VkSemaphoreCreateInfo semaphoreInfo = { VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO };
		VkFenceCreateInfo fenceInfo = { VK_STRUCTURE_TYPE_FENCE_CREATE_INFO };
		fenceInfo.flags = VK_FENCE_CREATE_SIGNALED_BIT;

		for (size_t i = 0; i < MAX_FRAMES_IN_FLIGHT; i++)
		{
			VK_CHECK(vkCreateSemaphore(handle, &semaphoreInfo, nullptr, &imageAvailableSemaphores[i]), "Failed to create semaphore.");
			VK_CHECK(vkCreateSemaphore(handle, &semaphoreInfo, nullptr, &renderFinishedSemaphores[i]), "Failed to create semaphore.");
			VK_CHECK(vkCreateFence(handle, &fenceInfo, nullptr, &inFlightFences[i]), "Failed to create fence.");
		}
	}

	void VulkanDevice::CreateCommandBuffers()
	{
		graphicsCommandBuffers.resize(MAX_FRAMES_IN_FLIGHT);
		computeCommandBuffers.resize(MAX_FRAMES_IN_FLIGHT);

		VkCommandBufferAllocateInfo graphicsAllocInfo = { VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO };
		graphicsAllocInfo.commandPool = graphicsCommandPool;
		graphicsAllocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
		graphicsAllocInfo.commandBufferCount = (uint32_t)graphicsCommandBuffers.size();

		VkCommandBufferAllocateInfo computeAllocInfo = { VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO };
		computeAllocInfo.commandPool = computeCommandPool;
		computeAllocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
		computeAllocInfo.commandBufferCount = (uint32_t)computeCommandBuffers.size();

		VK_CHECK(vkAllocateCommandBuffers(handle, &graphicsAllocInfo, graphicsCommandBuffers.data()), "Failed to allocate graphics command buffers.");
		VK_CHECK(vkAllocateCommandBuffers(handle, &computeAllocInfo, computeCommandBuffers.data()), "Failed to allocate compute command buffers.");
	}

	uint32_t VulkanDevice::FindMemoryType(uint32_t typeFilter, VkMemoryPropertyFlags properties)
	{
		VkPhysicalDeviceMemoryProperties memProperties;
		vkGetPhysicalDeviceMemoryProperties(physicalDevice->GetHandle(), &memProperties);

		for (uint32_t i = 0; i < memProperties.memoryTypeCount; i++)
		{
			if (typeFilter & (1 << i) && (memProperties.memoryTypes[i].propertyFlags & properties) == properties)
			{
				return i;
			}
		}

		LOG_ERROR("Failed to find suitable memory type.");

		return 0;
	}

	void VulkanDevice::CreateBuffer(VkDeviceSize size, VkBufferUsageFlags usage, VkMemoryPropertyFlags properties, VkBuffer &buffer, VkDeviceMemory& bufferMemory)
	{
		VkBufferCreateInfo bufferInfo = { VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO };
		bufferInfo.size = size;
		bufferInfo.usage = usage;

		std::vector<uint32_t> sharedIndices = { physicalDevice->queueFamilyIndices.graphicsFamily.value(), physicalDevice->queueFamilyIndices.transferFamily.value() };
		bufferInfo.sharingMode = VK_SHARING_MODE_CONCURRENT; // TODO: a memory barrier is more efficient than concurrent sharing mode
		bufferInfo.queueFamilyIndexCount = static_cast<uint32_t>(sharedIndices.size());
		bufferInfo.pQueueFamilyIndices = sharedIndices.data();

		VK_CHECK(vkCreateBuffer(handle, &bufferInfo, nullptr, &buffer), "Failed to create buffer.");

		VkMemoryRequirements memRequirements;
		vkGetBufferMemoryRequirements(handle, buffer, &memRequirements);

		VkMemoryAllocateInfo allocInfo = { VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO };
		allocInfo.allocationSize = memRequirements.size;
		allocInfo.memoryTypeIndex = FindMemoryType(memRequirements.memoryTypeBits, properties);

		VK_CHECK(vkAllocateMemory(handle, &allocInfo, nullptr, &bufferMemory), "Failed to allocate buffer memory.");
		vkBindBufferMemory(handle, buffer, bufferMemory, 0);
	}

	VkCommandBuffer VulkanDevice::BeginSingleTimeCommands(VkCommandPool commandPool)
	{
		VkCommandBufferAllocateInfo allocInfo = { VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO };
		allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
		allocInfo.commandPool = commandPool;
		allocInfo.commandBufferCount = 1;
		
		VkCommandBuffer commandBuffer;
		VK_CHECK(vkAllocateCommandBuffers(handle, &allocInfo, &commandBuffer), "Failed to allocate command buffer.");
		
		VkCommandBufferBeginInfo beginInfo = { VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO };
		beginInfo.flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;
		
		VK_CHECK(vkBeginCommandBuffer(commandBuffer, &beginInfo), "Failed to begin recording command buffer.");
		
		return commandBuffer;
	}

	void VulkanDevice::EndSingleTimeCommands(VkCommandBuffer commandBuffer, VkCommandPool commandPool, VkQueue queue)
	{
		VK_CHECK(vkEndCommandBuffer(commandBuffer), "Failed to record command buffer.");
		
		VkSubmitInfo submitInfo = { VK_STRUCTURE_TYPE_SUBMIT_INFO };
		submitInfo.commandBufferCount = 1;
		submitInfo.pCommandBuffers = &commandBuffer;
		
		vkQueueSubmit(queue, 1, &submitInfo, VK_NULL_HANDLE);
		vkQueueWaitIdle(queue);
		
		vkFreeCommandBuffers(handle, commandPool, 1, &commandBuffer);
	}

	void VulkanDevice::RecordGraphicsCommandBuffer(VkCommandBuffer commandBuffer, uint32_t imageIndex, VulkanModel* model, VulkanMaterial* material)
	{
		VkCommandBufferBeginInfo beginInfo = { VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO };
		beginInfo.flags = 0; // Optional
		beginInfo.pInheritanceInfo = nullptr; // Optional

		VK_CHECK(vkBeginCommandBuffer(commandBuffer, &beginInfo), "Failed to begin recording command buffer.");

		VkRenderPassBeginInfo renderPassInfo = { VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO };
		renderPassInfo.renderPass = swapchain->GetRenderPass()->GetHandle();
		renderPassInfo.framebuffer = swapchain->GetFramebuffer(imageIndex);
		renderPassInfo.renderArea.offset = {0, 0};
		VkExtent2D swapchainExtent = swapchain->GetExtent();
		renderPassInfo.renderArea.extent = swapchainExtent;
		
		std::array<VkClearValue, 2> clearValues{};
		clearValues[0].color = {{ 0.0f, 0.0f, 0.0f, 1.0f }};
		clearValues[1].depthStencil = { 1.0f, 0 };

		renderPassInfo.clearValueCount = static_cast<uint32_t>(clearValues.size());
		renderPassInfo.pClearValues = clearValues.data();

		vkCmdBeginRenderPass(commandBuffer, &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);
		vkCmdBindPipeline(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, material->GetPipeline());

		VkBuffer vertexBuffers[] = { model->GetVertexBuffer() };
		VkDeviceSize vertexBufferOffsets[] = { 0 };
		vkCmdBindVertexBuffers(commandBuffer, 0, 1, vertexBuffers, vertexBufferOffsets);
		vkCmdBindIndexBuffer(commandBuffer, model->GetIndexBuffer(), 0, VK_INDEX_TYPE_UINT32);

		VkDescriptorSet descriptorSet = material->GetDescriptorSet(currentFrame);
		vkCmdBindDescriptorSets(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, material->GetPipelineLayout(), 0, 1, &descriptorSet, 0, nullptr);

		VkViewport viewport{};
		viewport.x = 0.0f;
		viewport.y = 0.0f;
		viewport.width = static_cast<float>(swapchainExtent.width);
		viewport.height = static_cast<float>(swapchainExtent.height);
		viewport.minDepth = 0.0f;
		viewport.maxDepth = 1.0f;
		vkCmdSetViewport(commandBuffer, 0, 1, &viewport);
		
		VkRect2D scissor{};
		scissor.offset = {0, 0};
		scissor.extent = swapchainExtent;
		vkCmdSetScissor(commandBuffer, 0, 1, &scissor);

		vkCmdDrawIndexed(commandBuffer, static_cast<uint32_t>(model->indices.size()), 1, 0, 0, 0);
		vkCmdEndRenderPass(commandBuffer);

		VK_CHECK(vkEndCommandBuffer(commandBuffer), "Failed to record command buffer.");
	}

	void VulkanDevice::CopyBuffer(VkBuffer srcBuffer, VkBuffer dstBuffer, VkDeviceSize size)
	{
		VkCommandBuffer commandBuffer = BeginSingleTimeCommands(transferCommandPool);

		VkBufferCopy copyRegion{};
		copyRegion.srcOffset = 0;
		copyRegion.dstOffset = 0;
		copyRegion.size = size;
		vkCmdCopyBuffer(commandBuffer, srcBuffer, dstBuffer, 1, &copyRegion);

		EndSingleTimeCommands(commandBuffer, transferCommandPool, transferQueue.queue);
	}

	void VulkanDevice::CopyBufferToImage(VkBuffer buffer, VkImage image, uint32_t width, uint32_t height)
	{
		VkCommandBuffer commandBuffer = BeginSingleTimeCommands(transferCommandPool);

		VkBufferImageCopy region{};
		region.bufferOffset = 0;
		region.bufferRowLength = 0;
		region.bufferImageHeight = 0;
		region.imageSubresource.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
		region.imageSubresource.mipLevel = 0;
		region.imageSubresource.baseArrayLayer = 0;
		region.imageSubresource.layerCount = 1;
		region.imageOffset = { 0, 0, 0 };
		region.imageExtent = { width, height, 1 };

		vkCmdCopyBufferToImage(commandBuffer, buffer, image, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, 1, &region);

		EndSingleTimeCommands(commandBuffer, transferCommandPool, transferQueue.queue);
	}
}
