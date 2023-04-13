#pragma once

#include <vulkan/vulkan.h>

struct VmaAllocation_T;
VK_DEFINE_HANDLE(VmaAllocation)

struct AllocatedBuffer
{
    VkBuffer _buffer;
    VmaAllocation _allocation;
};

struct AllocatedImage {
    VkImage _image;
    VmaAllocation _allocation;
};
