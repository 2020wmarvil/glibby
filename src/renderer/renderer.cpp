#include "renderer.h"

#include "model.h"
#include "material.h"
#include "vulkan/vulkan_renderer.h"
#include "window.h"

namespace glibby
{
    Renderer::Renderer()
	{
		window = std::make_unique<Window>(800, 600, "Demo Window");
		renderer = std::make_unique<VulkanRenderer>(window.get());
		defaultMaterial = renderer->GetDefaultMaterial();
    }

    Renderer::~Renderer()
	{
		for (Model* model : models)
		{
			delete model;
		}
	}

	void Renderer::Run()
	{
        while (!window->ShouldClose())
        {
            window->PollEvents();
			renderer->DrawFrame(models, defaultMaterial);
        }

		renderer->WaitForIdle();
	}

	Model* Renderer::AddModelByFile(const char* filepath)
	{
		Model* model = renderer->LoadModelFromFile(filepath);
		models.insert(model);
		return model;
	}

	void Renderer::RemoveModel(Model* model)
	{
		models.erase(model);
	}
}
