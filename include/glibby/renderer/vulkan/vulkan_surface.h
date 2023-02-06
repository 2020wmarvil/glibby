#pragma once

#include "renderer_core.h"

namespace glibby
{
	class Window;
	class VulkanInstance;

	class VulkanSurface
	{
	public:
		VulkanSurface(VulkanInstance* instance, Window* window);
		~VulkanSurface();

		VkSurfaceKHR GetHandle() { return handle; }
		Window* GetWindow() { return window; }

	private:
		VulkanInstance* instance;
		Window* window;
		VkSurfaceKHR handle;
	};
}
