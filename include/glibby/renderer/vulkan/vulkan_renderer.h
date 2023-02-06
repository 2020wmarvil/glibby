#pragma once

#include "renderer_core.h"

#include <unordered_set>

// TODO: move vulkan directory out of the public include directory

namespace glibby
{
    class Window;
    class VulkanInstance;
    class VulkanSurface;
    class VulkanPhysicalDevice;
    class VulkanDevice;
    class VulkanRenderPass;
    class VulkanSwapchain;

    class Model;
    class Material;

    class VulkanRenderer
    {
    public:
        VulkanRenderer(Window* window);
        virtual ~VulkanRenderer();
    
        VulkanRenderer(VulkanRenderer const&) = delete;
        VulkanRenderer& operator=(VulkanRenderer other) = delete;

        Model* LoadModelFromFile(const char* filepath);
        Material* GetDefaultMaterial();

	    void DrawFrame(const std::unordered_set<Model*>& models, Material* material); // TODO: mark these as vulkanmodel and vulkanmaterial?
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
    };
}