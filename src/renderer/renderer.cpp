#include "renderer.h"

#include "camera.h"
#include "model.h"
#include "material.h"
#include "vulkan/vulkan_renderer.h"
#include "window.h"

namespace glibby
{
    Renderer::Renderer()
	{
		inputState = {};
		inputState.firstMouse = true;

		window = std::make_unique<Window>(800, 600, "Demo Window", &inputState);
		camera = std::make_unique<Camera>();
		renderer = std::make_unique<VulkanRenderer>(window.get(), camera.get());

		defaultMaterial = renderer->GetDefaultMaterial();
		lastFrameTime = window->GetTime();
    }

    Renderer::~Renderer()
	{
		for (auto& [material, models] : materialToModels)
		{
			for (Model* model : models)
			{
				delete model;
			}

			if (material != defaultMaterial)
			{
				delete material;
			}
		}
	}

	void Renderer::Run()
	{
		float currentFrameTime = window->GetTime();
        float deltaTime = currentFrameTime - lastFrameTime;
        lastFrameTime = currentFrameTime;

        while (!window->ShouldClose())
        {
			window->ProcessInput();
			camera->Update(inputState, deltaTime);
			renderer->DrawFrame(materialToModels);
            window->PollEvents();
        }

		renderer->WaitForIdle();
	}

	Model* Renderer::AddModelByFile(const char* filepath, Material* material)
	{
		Model* model = renderer->LoadModelFromFile(filepath);

		if (material)
		{
			materialToModels[material].insert(model);
		}
		else
		{
			materialToModels[defaultMaterial].insert(model);
		}

		return model;
	}

	Model* Renderer::AddModelByVertices(const std::vector<ModelVertex>& vertices, const std::vector<uint32_t>& indices, Material* material)
	{
		Model* model = renderer->CreateModel(vertices, indices);

		if (material)
		{
			materialToModels[material].insert(model);
		}
		else
		{
			materialToModels[defaultMaterial].insert(model);
		}

		return model;
	}

	void Renderer::RemoveModel(Model* model, Material* material)
	{
		if (material)
		{
			materialToModels[material].erase(model);
		}
		else
		{
			materialToModels[defaultMaterial].erase(model);
		}
	}
}
