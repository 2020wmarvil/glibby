#pragma once

#include "glm/glm.hpp"

#include <array>
#include <vector>

namespace glibby
{
	struct ModelVertex
	{
		glm::vec3 pos;
		glm::vec3 color;
		glm::vec2 texCoord;

		bool operator==(const ModelVertex& other) const;
	};

	class Model
	{
	public:
		Model(const char* filepath);
		Model(std::vector<ModelVertex> vertices, std::vector<uint32_t> indices);
		virtual ~Model();
	public:
		std::vector<ModelVertex> vertices;
		std::vector<uint32_t> indices;
	};
}
