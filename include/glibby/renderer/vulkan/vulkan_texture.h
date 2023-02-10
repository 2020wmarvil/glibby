#pragma once

#include "renderer_core.h"
#include "texture.h"

namespace glibby
{
	class VulkanDevice;
	class VulkanImage;

	class VulkanTexture : public Texture
	{
	public:
		VulkanTexture(VulkanDevice* device, void* pixelData, uint32_t texWidth, uint32_t texHeight, uint32_t mipLevels);
		~VulkanTexture();

		VulkanImage* GetImage() { return image.get(); }
		VkSampler GetSampler() { return textureSampler; }

	private:
		void CreateTextureImage(void* pixelData);
		void CreateTextureSampler();
		void GenerateMipmaps(VkImage image, VkFormat imageFormat, int32_t texWidth, int32_t texHeight, uint32_t mipLevels);

	private:
		VulkanDevice* device;
		std::unique_ptr<VulkanImage> image;
		VkSampler textureSampler;
		uint32_t texWidth;
		uint32_t texHeight;
		uint32_t mipLevels;
	};
}