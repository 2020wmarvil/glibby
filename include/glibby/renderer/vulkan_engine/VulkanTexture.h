#pragma once

#include "VulkanTypes.h"
#include "VulkanEngine.h"

namespace vkutil
{
	bool load_image_from_file(VulkanEngine& engine, const char* file, AllocatedImage& outImage);
}