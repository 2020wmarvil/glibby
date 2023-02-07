#pragma once

#include "renderer_core.h"
#include "model.h"

#include <unordered_set>
#include <unordered_map>

// TODO: move vulkan directory out of the public include directory

namespace glibby
{
    class Window;
    class Camera;
    class VulkanInstance;
    class VulkanSurface;
    class VulkanPhysicalDevice;
    class VulkanDevice;
    class VulkanRenderPass;
    class VulkanSwapchain;

    class Material;

    class VulkanRenderer
    {
    public:
        VulkanRenderer(Window* window, Camera* camera);
        virtual ~VulkanRenderer();
    
        VulkanRenderer(VulkanRenderer const&) = delete;
        VulkanRenderer& operator=(VulkanRenderer other) = delete;

        Model* LoadModelFromFile(const char* filepath);
        Model* CreateModel(const std::vector<ModelVertex>& vertices, const std::vector<uint32_t>& indices);
        Camera* GetCamera() { return camera; }
        Material* GetDefaultMaterial();

        void DrawFrame(const std::unordered_map<Material*, std::unordered_set<Model*>>& materialToModels);
	    void WaitForIdle();
    private:
	    void Initialize();
    private:
        std::unique_ptr<VulkanInstance> instance;
        std::unique_ptr<VulkanSurface> surface;
        std::unique_ptr<VulkanPhysicalDevice> physicalDevice;
        std::unique_ptr<VulkanDevice> device;
        std::unique_ptr<VulkanRenderPass> renderPass;
        std::unique_ptr<VulkanSwapchain> swapchain;

        std::unique_ptr<Material> defaultMaterial; // TODO: mark as vulkan material?

        Window* window;
        Camera* camera;
    };
}