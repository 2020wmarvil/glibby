#include "model.h"

#include "renderer_core.h"

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/hash.hpp>
#define TINYOBJLOADER_IMPLEMENTATION
#include <tiny_obj_loader.h>

#include <unordered_map>

namespace std
{
	template<> struct hash<glibby::ModelVertex>
	{
		size_t operator()(glibby::ModelVertex const& vertex) const
		{
			return ((hash<glm::vec3>()(vertex.pos) ^
				(hash<glm::vec3>()(vertex.color) << 1)) >> 1) ^
				(hash<glm::vec2>()(vertex.texCoord) << 1);
		}
	};
}

namespace glibby
{
	bool ModelVertex::operator==(const ModelVertex& other) const
	{
		return pos == other.pos && color == other.color && texCoord == other.texCoord;
	}

	Model::Model(const char* filepath)
	{
		tinyobj::attrib_t attrib;
		std::vector<tinyobj::shape_t> shapes;
		std::vector<tinyobj::material_t> materials;
		std::string warn, err;

		bool bSuccess = tinyobj::LoadObj(&attrib, &shapes, &materials, &warn, &err, filepath);

		if (!bSuccess)
		{
			LOG_ERROR(warn + err);
			return;
		}

		std::unordered_map<ModelVertex, uint32_t> uniqueVertices{};

		for (const tinyobj::shape_t& shape : shapes)
		{
		    for (const tinyobj::index_t& index : shape.mesh.indices)
			{
				ModelVertex vertex{};

				vertex.pos =
				{
					attrib.vertices[3 * index.vertex_index + 0],
					attrib.vertices[3 * index.vertex_index + 1],
					attrib.vertices[3 * index.vertex_index + 2]
				};

				vertex.texCoord =
				{
					attrib.texcoords[2 * index.texcoord_index + 0],
					1.0f - attrib.texcoords[2 * index.texcoord_index + 1]
				};
				
				vertex.color = { 1.0f, 1.0f, 1.0f };

				if (uniqueVertices.count(vertex) == 0)
				{
					uniqueVertices[vertex] = static_cast<uint32_t>(vertices.size());
					vertices.push_back(vertex);
				}

				indices.push_back(uniqueVertices[vertex]);
			}
		}
	}

	Model::Model(std::vector<ModelVertex> vertices, std::vector<uint32_t> indices)
	{
		this->vertices = vertices;
		this->indices = indices;
	}

	Model::~Model()
	{}
}
