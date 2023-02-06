#pragma once

#include "renderer_core.h"

#include "vulkan/vulkan_queue.h"

#include <optional>
#include <vector>

namespace glibby
{
	class VulkanInstance;
	class VulkanPhysicalDevice;
	class VulkanSwapchain;
	class VulkanModel;
	class VulkanMaterial;

	struct VulkanDeviceRequirements
	{
		std::vector<const char*> requiredDeviceExtensions;
		std::vector<const char*> requiredDeviceLayers;
	};

	class VulkanDevice
	{
	public:
		VulkanDevice(VulkanPhysicalDevice* physicalDevice, const VulkanDeviceRequirements& deviceRequirements);
		~VulkanDevice();

		void DrawFrame(VulkanModel* model, VulkanMaterial* material, bool framebufferResized);

		VkSampleCountFlagBits GetMaxUsableSampleCount();
		VkFormat FindSupportedFormat(const std::vector<VkFormat>& candidates, VkImageTiling tiling, VkFormatFeatureFlags features);
		VkFormat FindDepthFormat();
		bool HasStencilComponent(VkFormat format);

		void WaitForIdle()
		{
			vkDeviceWaitIdle(handle);
		}

		uint32_t FindMemoryType(uint32_t typeFilter, VkMemoryPropertyFlags properties);
		void CreateBuffer(VkDeviceSize size, VkBufferUsageFlags usage, VkMemoryPropertyFlags properties, VkBuffer& buffer, VkDeviceMemory& bufferMemory);
		VkCommandBuffer BeginSingleTimeCommands(VkCommandPool commandPool);
		void EndSingleTimeCommands(VkCommandBuffer commandBuffer, VkCommandPool commandPool, VkQueue queue);
		void RecordGraphicsCommandBuffer(VkCommandBuffer commandBuffer, uint32_t imageIndex, VulkanModel* model, VulkanMaterial* material);
		void CopyBuffer(VkBuffer srcBuffer, VkBuffer dstBuffer, VkDeviceSize size);
		void CopyBufferToImage(VkBuffer buffer, VkImage image, uint32_t width, uint32_t height);

		VkDevice GetHandle() { return handle; }
		VulkanPhysicalDevice* GetPhysicalDevice() { return physicalDevice; }
		void SetSwapchain(VulkanSwapchain* swapchain) { this->swapchain = swapchain; }

	private:
		void CreateCommandPools();
		void CreateDescriptorPools();
		void CreateSyncObjects();
		void CreateDescriptorSets(VulkanMaterial* material);
		void CreateCommandBuffers();
		
	public:
		VulkanPhysicalDevice* physicalDevice;
		VulkanQueue graphicsQueue;
		VulkanQueue presentQueue;
		VulkanQueue computeQueue;
		VulkanQueue transferQueue;
		VkCommandPool graphicsCommandPool;
		VkCommandPool transferCommandPool;
		VkCommandPool computeCommandPool;
		VkDescriptorPool descriptorPool;
	private:
		VkDevice handle;
		VulkanInstance* instance;
		VulkanSwapchain* swapchain;

		std::vector<VkCommandBuffer> graphicsCommandBuffers;
		std::vector<VkCommandBuffer> computeCommandBuffers;
		std::vector<VkSemaphore> imageAvailableSemaphores;
		std::vector<VkSemaphore> renderFinishedSemaphores;
		std::vector<VkFence> inFlightFences;

		uint32_t currentFrame = 0;
	};
}