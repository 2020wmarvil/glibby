#pragma once

#include "model.h"
#include "material.h"
#include "texture.h"
#include "input.h"

#include <memory>
#include <stdint.h>
#include <unordered_map>
#include <unordered_set>

namespace glibby
{
    class VulkanRenderer;
    class Window;
    class Camera;

    class Renderer
    {
    public:
        Renderer();
        virtual ~Renderer();
    
        Renderer(Renderer const&) = delete;
        Renderer& operator=(Renderer other) = delete;

        void Run();
        Model* AddModelByFile(const char* filepath, Material* material=nullptr);
        Model* AddModelByVertices(const std::vector<ModelVertex>& vertices, const std::vector<uint32_t>& indices, Material* material=nullptr);
        void RemoveModel(Model* model, Material* material=nullptr);

        Material* CreateMaterial(const PipelineInfo& pipelineInfo);
        Texture* CreateTexture(void* data, uint32_t texWidth, uint32_t texHeight, uint32_t mipLevels);
    private:
        std::unique_ptr<Window> window;
        std::unique_ptr<Camera> camera;
        std::unique_ptr<VulkanRenderer> renderer; // TODO: move this to a subclass
        InputState inputState;

        Material* defaultMaterial;
        std::unordered_map<Material*, std::unordered_set<Model*>> materialToModels;

		float lastFrameTime;
    };
}
