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
