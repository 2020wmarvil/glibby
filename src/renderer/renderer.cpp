#include "renderer.h"

#include "vulkan_impl.h"

namespace glibby
{
    Renderer::Renderer()
	{
		window = std::make_unique<Window>(800, 600, "Demo Window");
		Vulkan_Initialize(window.get());
    }

    Renderer::~Renderer() 
	{
		Vulkan_Cleanup();
		Window::Terminate();
	}

	void Renderer::Run()
	{
        while (!window->ShouldClose())
        {
            window->PollEvents();
			Vulkan_DrawFrame();
        }

		Vulkan_WaitForIdle();
	}
}
