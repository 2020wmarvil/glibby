cmake_minimum_required(VERSION 3.9)

project(VulkanMemoryAllocator LANGUAGES CXX)

find_package(Vulkan REQUIRED)
include_directories(${Vulkan_INCLUDE_DIR})

# VulkanMemoryAllocator contains an sample application which is not built by default
option(VMA_BUILD_SAMPLE "Build VulkanMemoryAllocator sample application" OFF)
option(VMA_BUILD_SAMPLE_SHADERS "Build VulkanMemoryAllocator sample application's shaders" OFF)

message(STATUS "VMA_BUILD_SAMPLE = ${VMA_BUILD_SAMPLE}")
message(STATUS "VMA_BUILD_SAMPLE_SHADERS = ${VMA_BUILD_SAMPLE_SHADERS}")

option(VMA_STATIC_VULKAN_FUNCTIONS "Link statically with Vulkan API" ON)
option(VMA_DYNAMIC_VULKAN_FUNCTIONS "Fetch pointers to Vulkan functions internally (no static linking)" OFF)
option(VMA_DEBUG_ALWAYS_DEDICATED_MEMORY "Every allocation will have its own memory block" OFF)
option(VMA_DEBUG_INITIALIZE_ALLOCATIONS "Automatically fill new allocations and destroyed allocations with some bit pattern" OFF)
option(VMA_DEBUG_GLOBAL_MUTEX "Enable single mutex protecting all entry calls to the library" OFF)
option(VMA_DEBUG_DONT_EXCEED_MAX_MEMORY_ALLOCATION_COUNT "Never exceed VkPhysicalDeviceLimits::maxMemoryAllocationCount and return error" OFF)

add_subdirectory(src)
