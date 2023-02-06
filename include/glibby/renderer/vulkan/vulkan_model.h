#pragma once

#include "renderer_core.h"
#include "model.h"

#include <array>
#include <vector>

namespace glibby
{
	class VulkanDevice;

	VkVertexInputBindingDescription GetVertexBindingDescription();
	std::array<VkVertexInputAttributeDescription, 3> GetVertexAttributeDescriptions();

	class VulkanModel : public Model
	{
	public:
		VulkanModel(VulkanDevice* device, const char* filepath);
		~VulkanModel();

		void CreateVertexBuffer();
		void CreateIndexBuffer();

		VkBuffer GetVertexBuffer() { return vertexBuffer; }
		VkBuffer GetIndexBuffer() { return indexBuffer; }

	private:
		VulkanDevice* device;

		VkBuffer vertexBuffer;
		VkDeviceMemory vertexBufferMemory;
		VkBuffer indexBuffer;
		VkDeviceMemory indexBufferMemory;
	};
}
