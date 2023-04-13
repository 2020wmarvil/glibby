cmake_minimum_required(VERSION 3.10 FATAL_ERROR)
project(VulkanBootstrap)

find_package(Vulkan QUIET)
add_library(vkbootstrap VkBootstrap.h VkBootstrap.cpp VkBootstrapDispatch.h)
target_link_libraries(vkbootstrap ${Vulkan_LIBRARY})
target_include_directories(vkbootstrap PRIVATE "${Vulkan_INCLUDE_DIRS}")
