#include "vulkan/vulkan_model.h"

#include "vulkan/vulkan_device.h"

namespace glibby
{

	VkVertexInputBindingDescription GetVertexBindingDescription()
	{
		VkVertexInputBindingDescription bindingDescription{};
		bindingDescription.binding = 0;
		bindingDescription.stride = sizeof(ModelVertex);
		bindingDescription.inputRate = VK_VERTEX_INPUT_RATE_VERTEX;
		return bindingDescription;
	}

	std::array<VkVertexInputAttributeDescription, 3> GetVertexAttributeDescriptions()
	{
		std::array<VkVertexInputAttributeDescription, 3> attributeDescriptions{};

		attributeDescriptions[0].binding = 0;
		attributeDescriptions[0].location = 0;
		attributeDescriptions[0].format = VK_FORMAT_R32G32B32_SFLOAT;
		attributeDescriptions[0].offset = offsetof(ModelVertex, pos);

		attributeDescriptions[1].binding = 0;
		attributeDescriptions[1].location = 1;
		attributeDescriptions[1].format = VK_FORMAT_R32G32B32_SFLOAT;
		attributeDescriptions[1].offset = offsetof(ModelVertex, color);

		attributeDescriptions[2].binding = 0;
		attributeDescriptions[2].location = 2;
		attributeDescriptions[2].format = VK_FORMAT_R32G32_SFLOAT;
		attributeDescriptions[2].offset = offsetof(ModelVertex, texCoord);

		return attributeDescriptions;
	}

	VulkanModel::VulkanModel(VulkanDevice* device, const char* filepath)
		: Model(filepath)
	{
		this->device = device;

		CreateVertexBuffer();
		CreateIndexBuffer();
	}

	VulkanModel::VulkanModel(VulkanDevice* device, const std::vector<ModelVertex>& vertices, const std::vector<uint32_t>& indices)
		: Model(vertices, indices)
	{
		this->device = device;

		CreateVertexBuffer();
		CreateIndexBuffer();
	}

	VulkanModel::~VulkanModel()
	{
		vkDestroyBuffer(device->GetHandle(), vertexBuffer, nullptr);
		vkFreeMemory(device->GetHandle(), vertexBufferMemory, nullptr);
		vkDestroyBuffer(device->GetHandle(), indexBuffer, nullptr);
		vkFreeMemory(device->GetHandle(), indexBufferMemory, nullptr);
	}

	void VulkanModel::CreateVertexBuffer()
	{
		VkDeviceSize bufferSize = sizeof(vertices[0]) * vertices.size();

		VkBuffer stagingBuffer;
		VkDeviceMemory stagingBufferMemory;
		device->CreateBuffer(bufferSize, VK_BUFFER_USAGE_TRANSFER_SRC_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, stagingBuffer, stagingBufferMemory);

		void* data;
		vkMapMemory(device->GetHandle(), stagingBufferMemory, 0, bufferSize, 0, &data);
		memcpy(data, vertices.data(), bufferSize);
		vkUnmapMemory(device->GetHandle(), stagingBufferMemory);

		device->CreateBuffer(bufferSize, VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_VERTEX_BUFFER_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, vertexBuffer, vertexBufferMemory);
		device->CopyBuffer(stagingBuffer, vertexBuffer, bufferSize);

		vkDestroyBuffer(device->GetHandle(), stagingBuffer, nullptr);
		vkFreeMemory(device->GetHandle(), stagingBufferMemory, nullptr);
	}

	void VulkanModel::CreateIndexBuffer()
	{
		VkDeviceSize bufferSize = sizeof(indices[0]) * indices.size();

		VkBuffer stagingBuffer;
		VkDeviceMemory stagingBufferMemory;
		device->CreateBuffer(bufferSize, VK_BUFFER_USAGE_TRANSFER_SRC_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, stagingBuffer, stagingBufferMemory);

		void* data;
		vkMapMemory(device->GetHandle(), stagingBufferMemory, 0, bufferSize, 0, &data);
		memcpy(data, indices.data(), bufferSize);
		vkUnmapMemory(device->GetHandle(), stagingBufferMemory);

		device->CreateBuffer(bufferSize, VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_INDEX_BUFFER_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, indexBuffer, indexBufferMemory);
		device->CopyBuffer(stagingBuffer, indexBuffer, bufferSize);

		vkDestroyBuffer(device->GetHandle(), stagingBuffer, nullptr);
		vkFreeMemory(device->GetHandle(), stagingBufferMemory, nullptr);
	}
}
