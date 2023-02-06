#include "vulkan/vulkan_instance.h"

#include "window.h"
#include "vulkan/vulkan_physical_device.h"
#include "vulkan/vulkan_queue.h"
#include "vulkan/vulkan_swapchain.h"

#include <unordered_set>

#ifdef GLIBBY_DEBUG
static VKAPI_ATTR VkBool32 VKAPI_CALL VulkanDebugCallback(
	VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
	VkDebugUtilsMessageTypeFlagsEXT messageType,
	const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData,
	void* pUserData)
{
	if (messageSeverity >= VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT)
	{
		std::cerr << "\033[31m" << "Validation Layer: " << pCallbackData->pMessage << "\033[0m" << std::endl;
	}
	else if (messageSeverity >= VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT)
	{
		std::cerr << "\033[33m" << "Validation Layer: " << pCallbackData->pMessage << "\033[0m" << std::endl;
	}
	else
	{
		std::cerr << "Validation Layer: " << pCallbackData->pMessage << std::endl;
	}

	return VK_FALSE;
}
#endif

namespace glibby
{
#ifdef GLIBBY_DEBUG
	static void PopulateDebugMessengerCreateInfo(VkDebugUtilsMessengerCreateInfoEXT& createInfo) {
		createInfo = { VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT };
		createInfo.messageSeverity =
			//VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT |
			//VK_DEBUG_UTILS_MESSAGE_SEVERITY_INFO_BIT_EXT |
			VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT |
			VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT;
		createInfo.messageType = 
			VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT |
			VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT |
			VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT;
		createInfo.pfnUserCallback = VulkanDebugCallback;
		createInfo.pUserData = nullptr;
	}

	void VulkanInstance::SetupDebugMessenger()
	{
		VkDebugUtilsMessengerCreateInfoEXT createInfo;
		PopulateDebugMessengerCreateInfo(createInfo);

		auto createDebugUtilsMessengerFunc = (PFN_vkCreateDebugUtilsMessengerEXT)vkGetInstanceProcAddr(handle, "vkCreateDebugUtilsMessengerEXT");
		if (createDebugUtilsMessengerFunc != nullptr)
		{
			VK_CHECK(createDebugUtilsMessengerFunc(handle, &createInfo, nullptr, &debugMessenger), "Failed to create debug messenger.");
		}
		else
		{
			LOG_ERROR("Failed to acquire CreateDebugUtilsMessengerExtension.");
		}
	}
#endif
}

namespace glibby
{
	VulkanInstance::VulkanInstance(const VulkanInstanceRequirements& instanceRequirements)
	{
		VkApplicationInfo appInfo = { VK_STRUCTURE_TYPE_APPLICATION_INFO };
		appInfo.pApplicationName = "glibby Renderer";
		appInfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
		appInfo.pEngineName = "No Engine";
		appInfo.engineVersion = VK_MAKE_VERSION(1, 0, 0);
		appInfo.apiVersion = VK_API_VERSION_1_3;

		VkInstanceCreateInfo createInfo = { VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO };
		createInfo.pApplicationInfo = &appInfo;
		//createInfo.flags = VK_INSTANCE_CREATE_ENUMERATE_PORTABILITY_BIT_KHR; // Necessary for MacOS?

		uint32_t windowExtensionCount;
		const char** windowExtensions = nullptr;
		Window::GetRequiredWindowExtensions(windowExtensions, windowExtensionCount);

		std::vector<const char*> requiredExtensions = instanceRequirements.requiredBaseExtensions;
		for (int i = 0; i < windowExtensionCount; i++)
		{
			requiredExtensions.push_back(windowExtensions[i]);
		}

		{ // Verify required extensions are available
			uint32_t availableExtensionCount = 0;
			vkEnumerateInstanceExtensionProperties(nullptr, &availableExtensionCount, nullptr);
			std::vector<VkExtensionProperties> availableExtensions(availableExtensionCount);
			vkEnumerateInstanceExtensionProperties(nullptr, &availableExtensionCount, availableExtensions.data());

			std::unordered_set<std::string> availableExtensionSet;
			for (const VkExtensionProperties& availableExtension : availableExtensions) {
				availableExtensionSet.emplace(availableExtension.extensionName);
			}

			for (const char* requiredExtension : requiredExtensions)
			{
				if (availableExtensionSet.find(requiredExtension) == availableExtensionSet.end())
				{
					LOG_ERROR("Required extension " << requiredExtension << " is not available.");
				}
			}
		}

		createInfo.enabledExtensionCount = requiredExtensions.size();
		createInfo.ppEnabledExtensionNames = requiredExtensions.data();;
		
#ifdef GLIBBY_DEBUG
		{ // Verify required layers are available
			uint32_t layerCount;
			vkEnumerateInstanceLayerProperties(&layerCount, nullptr);	
			std::vector<VkLayerProperties> availableLayers(layerCount);
			vkEnumerateInstanceLayerProperties(&layerCount, availableLayers.data());

			std::unordered_set<std::string> availableLayerSet;
			for (const VkLayerProperties& availableLayer : availableLayers) {
				availableLayerSet.emplace(availableLayer.layerName);
			}

			for (const char* requiredLayer : instanceRequirements.requiredLayers)
			{
				if (availableLayerSet.find(requiredLayer) == availableLayerSet.end())
				{
					LOG_ERROR("Required layer " << requiredLayer << " is not available.");
				}
			}
		}

		createInfo.enabledLayerCount = static_cast<uint32_t>(instanceRequirements.requiredLayers.size());
		createInfo.ppEnabledLayerNames = instanceRequirements.requiredLayers.data();

		VkDebugUtilsMessengerCreateInfoEXT debugCreateInfo;
		PopulateDebugMessengerCreateInfo(debugCreateInfo);
		createInfo.pNext = (VkDebugUtilsMessengerCreateInfoEXT*)&debugCreateInfo;
#else
		createInfo.enabledLayerCount = 0;
#endif

		// TODO: MoltenVK support: https://vulkan-tutorial.com/Drawing_a_triangle/Setup/Instance

		VK_CHECK(vkCreateInstance(&createInfo, nullptr, &handle), "Failed to create Vulkan instance.");
	}

	VulkanInstance::~VulkanInstance()
	{
#ifdef GLIBBY_DEBUG
		auto destroyDebugUtilsFunc = (PFN_vkDestroyDebugUtilsMessengerEXT) vkGetInstanceProcAddr(handle, "vkDestroyDebugUtilsMessengerEXT");
		if (destroyDebugUtilsFunc)
		{
			destroyDebugUtilsFunc(handle, debugMessenger, nullptr);
		}
		else
		{
			LOG_ERROR("Failed to acquire DestroyDebugUtilsMessengerExtension.");
		}
#endif
		vkDestroyInstance(handle, nullptr);
	}
}
