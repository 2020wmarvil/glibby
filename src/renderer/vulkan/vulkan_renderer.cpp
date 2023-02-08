#include "vulkan/vulkan_renderer.h"

#include "camera.h"
#include "model.h"
#include "renderer_core.h"
#include "vulkan/vulkan_device.h"
#include "vulkan/vulkan_instance.h"
#include "vulkan/vulkan_material.h"
#include "vulkan/vulkan_model.h"
#include "vulkan/vulkan_physical_device.h"
#include "vulkan/vulkan_pipeline.h"
#include "vulkan/vulkan_render_pass.h"
#include "vulkan/vulkan_surface.h"
#include "vulkan/vulkan_swapchain.h"
#include "window.h"

#include <algorithm>
#include <array>
#include <chrono>
#include <fstream>
#include <optional>
#include <set>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <vector>

namespace glibby
{
	// TODO: potentially cleaner solution to callbacks can be found at: https://vulkan-tutorial.com/en/Drawing_a_triangle/Swap_chain_recreation 
	bool framebufferResized = false;
	void WindowFramebufferResizeCallback(int width, int height)
	{
		framebufferResized = true;
	}

	VulkanRenderer::VulkanRenderer(Window* window, Camera* camera)
		: window(window), camera(camera)
	{
		Initialize();

		PipelineInfo defaultPipelineInfo = { "../../resources/shaders/defaultVert.spv", "../../resources/shaders/defaultFrag.spv" };
		defaultMaterial = std::make_unique<VulkanMaterial>(device.get(), renderPass.get(), defaultPipelineInfo);
	}

	VulkanRenderer::~VulkanRenderer()
	{}

	Model* VulkanRenderer::LoadModelFromFile(const char* filepath)
	{
		return new VulkanModel(device.get(), filepath);
	}

	Model* VulkanRenderer::CreateModel(const std::vector<ModelVertex>& vertices, const std::vector<uint32_t>& indices)
	{
		return new VulkanModel(device.get(), vertices, indices);
	}

	Material* VulkanRenderer::CreateMaterial(const PipelineInfo& pipelineInfo)
	{
		return new VulkanMaterial(device.get(), renderPass.get(), pipelineInfo);
	}

	Material* VulkanRenderer::GetDefaultMaterial()
	{
		return defaultMaterial.get();
	}
	
	void VulkanRenderer::DrawFrame(const std::unordered_map<Material*, std::unordered_set<Model*>>& materialToModels)
	{
		for (auto& [material, models] : materialToModels)
		{
			for (Model* model : models)
			{
				device->DrawFrame(dynamic_cast<VulkanModel*>(model), dynamic_cast<VulkanMaterial*>(material), framebufferResized);
			}
		}
	}

	void VulkanRenderer::WaitForIdle()
	{
		device->WaitForIdle();
	}

	static const std::vector<const char*> requiredBaseExtensions =
	{
		//VK_KHR_PORTABILITY_ENUMERATION_EXTENSION_NAME, // Necessary for MacOS?
	#ifdef GLIBBY_DEBUG
		VK_EXT_DEBUG_UTILS_EXTENSION_NAME,
	#endif
	};
	
	#ifdef GLIBBY_DEBUG
	static const std::vector<const char*> requiredLayers =
	{
		"VK_LAYER_KHRONOS_validation"
	};
	#endif

	static const std::vector<const char*> requiredDeviceExtensions =
	{
		VK_KHR_SWAPCHAIN_EXTENSION_NAME
	};

	void VulkanRenderer::Initialize()
	{
		window->SetFramebufferResizeCallback(WindowFramebufferResizeCallback);

		VulkanInstanceRequirements instanceRequirements{};
		instanceRequirements.requiredBaseExtensions = requiredBaseExtensions;
		instanceRequirements.requiredLayers = requiredLayers;
		instance = std::make_unique<VulkanInstance>(instanceRequirements);
#ifdef GLIBBY_DEBUG
		instance->SetupDebugMessenger();
#endif

		surface = std::make_unique<VulkanSurface>(instance.get(), window);

		VulkanPhysicalDeviceRequirements physicalDeviceRequirements{};
		physicalDeviceRequirements.surface = surface.get();
		physicalDeviceRequirements.requiredDeviceExtensions = requiredDeviceExtensions;
		physicalDevice = std::make_unique<VulkanPhysicalDevice>(instance.get(), physicalDeviceRequirements);

		VulkanDeviceRequirements deviceRequirements{};
		deviceRequirements.requiredDeviceExtensions = requiredDeviceExtensions;
		deviceRequirements.requiredDeviceLayers = requiredLayers;
		device = std::make_unique<VulkanDevice>(this, physicalDevice.get(), deviceRequirements);

		SwapchainSupportDetails swapchainSupport = QuerySwapchainSupport(physicalDevice->GetHandle(), surface.get());
		VkSurfaceFormatKHR surfaceFormat = physicalDevice->ChooseSwapSurfaceFormat(swapchainSupport.formats);

		renderPass = std::make_unique<VulkanRenderPass>(device.get(), surfaceFormat.format);
		swapchain = std::make_unique<VulkanSwapchain>(device.get(), physicalDevice.get(), surface.get(), renderPass.get());

		device->SetSwapchain(swapchain.get());
	}
}
