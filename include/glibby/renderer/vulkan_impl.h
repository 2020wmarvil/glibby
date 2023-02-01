#pragma once

#include "vulkan/vulkan.h"

class Window;

namespace glibby
{
	void Vulkan_Initialize(Window* window);
	void Vulkan_Cleanup();
	void Vulkan_DrawFrame();
	void Vulkan_WaitForIdle();
}

// Vulkan_Shader
// Vulkan_VertexBuffer
// Vulkan_IndexBuffer
// Vulkan_UniformBuffer
// Vulkan_Texture
