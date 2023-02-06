#pragma once

#include "renderer_core.h"

namespace glibby
{
	class VulkanDevice;

	VkImageView CreateImageView(VkDevice device, VkImage image, VkFormat format, VkImageAspectFlags aspectFlags, uint32_t mipLevels);

	class VulkanImage
	{
	public:
		VulkanImage(VulkanDevice* device, uint32_t width, uint32_t height, uint32_t mipLevels, VkSampleCountFlagBits numSamples,
					VkFormat format, VkImageTiling tiling, VkImageUsageFlags usage, VkImageAspectFlags aspectFlags, VkMemoryPropertyFlags properties);
		~VulkanImage();

		VkImage GetHandle() { return handle; }
		VkImageView GetImageView() { return imageView; }

		void TransitionImageLayout(VkFormat format, VkImageLayout oldLayout, VkImageLayout newLayout, uint32_t mipLevels);
		void GenerateMipmaps(VkFormat imageFormat, int32_t texWidth, int32_t texHeight, uint32_t mipLevels);
	private:
		VulkanDevice* device;
		VkImage handle;
		VkDeviceMemory imageMemory;
		VkImageView imageView;
	};
}
