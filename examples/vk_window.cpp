#include "glibby/renderer/vulkan_engine/VulkanEngine.h"
#include "glibby/renderer/Window.h"

int main()
{
	Window::Init();

    VulkanEngine engine;
	engine.init();
	engine.run();
	engine.cleanup();

	Window::Terminate();

	return 0;
}