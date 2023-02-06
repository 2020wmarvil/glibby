#include "vulkan/vulkan_texture.h"

#include "vulkan/vulkan_device.h"
#include "vulkan/vulkan_physical_device.h"
#include "vulkan/vulkan_image.h"

namespace glibby
{
	VulkanTexture::VulkanTexture(VulkanDevice* device, void* pixelData, uint32_t texWidth, uint32_t texHeight, uint32_t mipLevels)
	{
		this->device = device;
		this->texWidth = texWidth;
		this->texHeight = texHeight;
		this->mipLevels = mipLevels;

		CreateTextureImage(pixelData);
		CreateTextureSampler();
	}

	VulkanTexture::~VulkanTexture()
	{
		vkDestroySampler(device->GetHandle(), textureSampler, nullptr);
	}

	void VulkanTexture::CreateTextureImage(void* pixelData)
	{
		VkDeviceSize imageSize = texWidth * texHeight * 4;

		VkBuffer stagingBuffer;
		VkDeviceMemory stagingBufferMemory;
		device->CreateBuffer(imageSize, VK_BUFFER_USAGE_TRANSFER_SRC_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, stagingBuffer, stagingBufferMemory);

		void* data;
		vkMapMemory(device->GetHandle(), stagingBufferMemory, 0, imageSize, 0, &data);
		memcpy(data, pixelData, static_cast<size_t>(imageSize));
		vkUnmapMemory(device->GetHandle(), stagingBufferMemory);

		VkImageUsageFlags usage = VK_IMAGE_USAGE_TRANSFER_DST_BIT | VK_IMAGE_USAGE_SAMPLED_BIT;
		if (mipLevels > 1)
		{
			usage |= VK_IMAGE_USAGE_TRANSFER_SRC_BIT;
		}

		image = std::make_unique<VulkanImage>(device, texWidth, texHeight, mipLevels, 
			VK_SAMPLE_COUNT_1_BIT, VK_FORMAT_R8G8B8A8_SRGB, VK_IMAGE_TILING_OPTIMAL, usage, VK_IMAGE_ASPECT_COLOR_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT);

		image->TransitionImageLayout(VK_FORMAT_R8G8B8A8_SRGB, VK_IMAGE_LAYOUT_UNDEFINED, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, mipLevels); // expose image for copying to
		device->CopyBufferToImage(stagingBuffer, image->GetHandle(), static_cast<uint32_t>(texWidth), static_cast<uint32_t>(texHeight));

		if (mipLevels == 1)
		{
			image->TransitionImageLayout(VK_FORMAT_R8G8B8A8_SRGB, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL, mipLevels); // expose image to be read from the shader
		}
		else
		{
			image->GenerateMipmaps(VK_FORMAT_R8G8B8A8_SRGB, texWidth, texHeight, mipLevels);
		}	

		vkDestroyBuffer(device->GetHandle(), stagingBuffer, nullptr);
		vkFreeMemory(device->GetHandle(), stagingBufferMemory, nullptr);
	}

	void VulkanTexture::CreateTextureSampler()
	{
		VkSamplerCreateInfo samplerInfo = { VK_STRUCTURE_TYPE_SAMPLER_CREATE_INFO };
		samplerInfo.magFilter = VK_FILTER_LINEAR;
		samplerInfo.minFilter = VK_FILTER_LINEAR;
		samplerInfo.addressModeU = VK_SAMPLER_ADDRESS_MODE_REPEAT;
		samplerInfo.addressModeV = VK_SAMPLER_ADDRESS_MODE_REPEAT;
		samplerInfo.addressModeW = VK_SAMPLER_ADDRESS_MODE_REPEAT;
		samplerInfo.borderColor = VK_BORDER_COLOR_INT_OPAQUE_BLACK;
		samplerInfo.unnormalizedCoordinates = VK_FALSE;
		samplerInfo.compareEnable = VK_FALSE;
		samplerInfo.compareOp = VK_COMPARE_OP_ALWAYS;
		samplerInfo.mipmapMode = VK_SAMPLER_MIPMAP_MODE_LINEAR;
		samplerInfo.minLod = 0.0f;
		samplerInfo.maxLod = static_cast<float>(mipLevels);
		samplerInfo.mipLodBias = 0.0f;

		if (device->GetPhysicalDevice()->features.samplerAnisotropy)
		{
			samplerInfo.anisotropyEnable = VK_TRUE;
			samplerInfo.maxAnisotropy = device->GetPhysicalDevice()->properties.limits.maxSamplerAnisotropy;
		}
		else
		{
			samplerInfo.anisotropyEnable = VK_FALSE;
			samplerInfo.maxAnisotropy = 1.0f;
		}

		VK_CHECK(vkCreateSampler(device->GetHandle(), &samplerInfo, nullptr, &textureSampler), "Failed to create texture sampler.");
	}
}