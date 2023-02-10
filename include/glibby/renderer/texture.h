#pragma once

#include <memory>

namespace glibby
{
	void* LoadTextureData(const char* filepath, int* texWidth, int* texHeight, int* mipLevels);
	void FreeTextureData(void* pixels);

	class Texture
	{
	public:
		Texture();
		virtual ~Texture();
	};
}