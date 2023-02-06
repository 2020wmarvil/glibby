#include "texture.h"

#include "renderer_core.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#include <cmath>

namespace glibby
{
	void* LoadTextureData(const char* filepath, int* texWidth, int* texHeight, int* mipLevels)
	{
		int texChannels;
		stbi_uc* pixels = stbi_load(filepath, texWidth, texHeight, &texChannels, STBI_rgb_alpha);
		*mipLevels = static_cast<int>(std::floor(std::log2(std::max(*texWidth, *texHeight)))) + 1;

		ASSERT(pixels != nullptr, "Failed to load image.");

		return pixels;
	}

	void FreeTextureData(void* pixels)
	{
		stbi_image_free(pixels);
	}
}