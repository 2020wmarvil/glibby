#pragma once

#include <memory>
#include <stdint.h>
#include <unordered_set>

namespace glibby
{
    class VulkanRenderer;
    class Window;
    class Model;
    class Material;

    class Renderer
    {
    public:
        Renderer();
        virtual ~Renderer();
    
        Renderer(Renderer const&) = delete;
        Renderer& operator=(Renderer other) = delete;

        void Run();
        Model* AddModelByFile(const char* filepath);
        void RemoveModel(Model* model);
    private:
        std::unique_ptr<Window> window;
        std::unique_ptr<VulkanRenderer> renderer;
        Material* defaultMaterial;
        std::unordered_set<Model*> models;
    };
}
