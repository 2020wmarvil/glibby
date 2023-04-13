#pragma once

#include "VulkanTypes.h"
#include "VulkanMesh.h"
#include "../Window.h"


#include "glm/glm.hpp"
#include "glm/gtx/transform.hpp"

#include <deque>
#include <functional>
#include <memory>
#include <unordered_map>
#include <vector>

/* TODO:
 * - Clean up all the hardcoded pipelines and meshes from VulkanEngine class
 * - Add WASD controls to the camera. For that, you would need to modify the camera matrices in the draw functions.
 * - Sort the renderables array before rendering by Pipeline and Mesh, to reduce number of binds.
 * - Instanced rendering + per instance custom data
 * - Cache Vulkan objects (Pipelines, render passes, samplers)
 */

struct VmaAllocator_T;
VK_DEFINE_HANDLE(VmaAllocator)

enum VmaMemoryUsage : int;

//number of frames to overlap when rendering
constexpr unsigned int FRAME_OVERLAP = 2;

struct Texture
{
	AllocatedImage image;
	VkImageView imageView;
};

struct Material
{
	VkDescriptorSet textureSet{VK_NULL_HANDLE};
	VkPipeline pipeline;
	VkPipelineLayout pipelineLayout;
};

struct RenderObject
{
	Mesh* mesh;
	Material* material;
	glm::mat4 transformMatrix;
};

struct GPUCameraData
{
	glm::mat4 view;
	glm::mat4 proj;
	glm::mat4 viewproj;
};

struct GPUSceneData
{
	glm::vec4 fogColor; // w is for exponent
	glm::vec4 fogDistances; //x for min, y for max, zw unused.
	glm::vec4 ambientColor;
	glm::vec4 sunlightDirection; //w for sun power
	glm::vec4 sunlightColor;
};

struct GPUObjectData
{
	glm::mat4 modelMatrix;
};

struct FrameData
{
	VkSemaphore _presentSemaphore, _renderSemaphore;
	VkFence _renderFence;	
	VkCommandPool _commandPool;
	VkCommandBuffer _mainCommandBuffer;

	AllocatedBuffer cameraBuffer; // buffer that holds a single GPUCameraData to use when rendering
	VkDescriptorSet globalDescriptor; // descriptor 0

	AllocatedBuffer objectBuffer; // buffer that holds a object data from the scene
	VkDescriptorSet objectDescriptor; // descriptor 1
};

struct DeletionQueue
{
	std::deque<std::function<void()>> deletors;

	void push_function(std::function<void()>&& function) { deletors.push_back(function); }

	void flush()
	{
		for (auto it = deletors.rbegin(); it != deletors.rend(); it++)
		{ 
			(*it)(); 
		}

		deletors.clear();
	}
};

struct UploadContext {
	VkFence _uploadFence;
	VkCommandPool _commandPool;
	VkCommandBuffer _commandBuffer;
};

class PipelineBuilder
{
public:
	std::vector<VkPipelineShaderStageCreateInfo> _shaderStages;
	VkPipelineVertexInputStateCreateInfo _vertexInputInfo;
	VkPipelineInputAssemblyStateCreateInfo _inputAssembly;
	VkViewport _viewport;
	VkRect2D _scissor;
	VkPipelineRasterizationStateCreateInfo _rasterizer;
	VkPipelineColorBlendAttachmentState _colorBlendAttachment;
	VkPipelineMultisampleStateCreateInfo _multisampling;
	VkPipelineDepthStencilStateCreateInfo _depthStencil;
	VkPipelineLayout _pipelineLayout;

	VkPipeline build_pipeline(VkDevice device, VkRenderPass pass);
};

class VulkanEngine
{
public:
	bool _isInitialized{ false };
	int _frameNumber {0};

	VkExtent2D _windowExtent{ 1700 , 900 };
	std::unique_ptr<Window> _window{ nullptr };

	VkInstance _instance;
	VkDebugUtilsMessengerEXT _debug_messenger;
	VkPhysicalDevice _chosenGPU;
	VkPhysicalDeviceProperties _gpuProperties;
	VkDevice _device;
	VkSurfaceKHR _surface;
	VkQueue _graphicsQueue;
	uint32_t _graphicsQueueFamily;

	VkSwapchainKHR _swapchain;
	VkFormat _swapchainImageFormat;
	std::vector<VkImage> _swapchainImages;
	std::vector<VkImageView> _swapchainImageViews;

	VkImageView _depthImageView;
	AllocatedImage _depthImage;
	VkFormat _depthFormat;

	VkRenderPass _renderPass;
	std::vector<VkFramebuffer> _framebuffers;

	VkDescriptorPool _descriptorPool;
	VkDescriptorSetLayout _globalSetLayout; // descriptor 0
	VkDescriptorSetLayout _objectSetLayout; // descriptor 1
	VkDescriptorSetLayout _singleTextureSetLayout; // descriptor 2

	VmaAllocator _allocator;
	DeletionQueue _mainDeletionQueue;

	FrameData _frames[FRAME_OVERLAP];

	GPUSceneData _sceneParameters;
	AllocatedBuffer _sceneParameterBuffer;

	std::vector<RenderObject> _renderables;
	std::unordered_map<std::string, Material> _materials;
	std::unordered_map<std::string, Mesh> _meshes;
	std::unordered_map<std::string, Texture> _loadedTextures;

	UploadContext _uploadContext;
	
	void init();
	void init_vulkan();
	void init_swapchain();
	void init_commands();
	void init_default_renderpass();
	void init_framebuffers();
	void init_sync_structures();
	void init_descriptors();
	void init_pipelines();
	void init_scene();

	void cleanup();
	void draw();
	void run();

	bool load_shader_module(const char* filePath, VkShaderModule* outShaderModule);
	void load_meshes();
	void load_images();
	void upload_mesh(Mesh& mesh);

	Material* create_material(VkPipeline pipeline, VkPipelineLayout layout, const std::string& name);
	Material* get_material(const std::string& name);
	Mesh* get_mesh(const std::string& name);
	void draw_objects(VkCommandBuffer cmd,RenderObject* first, int count);

	FrameData& get_current_frame();
	AllocatedBuffer create_buffer(size_t allocSize, VkBufferUsageFlags usage, VmaMemoryUsage memoryUsage);
	size_t pad_uniform_buffer_size(size_t originalSize);
	void immediate_submit(std::function<void(VkCommandBuffer cmd)>&& function);
};
