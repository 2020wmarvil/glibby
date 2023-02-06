#include "vulkan/vulkan_surface.h"

#include "vulkan/vulkan_instance.h"
#include "window.h"

namespace glibby
{
	VulkanSurface::VulkanSurface(VulkanInstance* instance, Window* window)
		: instance(instance), window(window)
	{
		VK_CHECK(window->CreateVulkanWindowSurface(instance->GetHandle(), &handle), "Failed to create window surface.");
	}

	VulkanSurface::~VulkanSurface()
	{
		vkDestroySurfaceKHR(instance->GetHandle(), handle, nullptr);
	}
}