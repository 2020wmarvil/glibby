#include "glibby/renderer/renderer.h"
#include "glibby/renderer/window.h"
#include "glibby/renderer/model.h"
#include "glibby/renderer/material.h"

#include "glibby/primitives/point2D.h"

#include <vector>

using namespace glibby;

void PackPoints(const std::vector<Point2D>& inPoints, std::vector<ModelVertex>& outVertices, std::vector<uint32_t>& outIndices)
{
	const float halfWidth = 0.5f;

	outVertices.resize(inPoints.size() * 4); // one quad per point
	outIndices.resize(inPoints.size() * 6); // 2 triangles of 3 indices per quad

	uint32_t indexCount = 0;

	for (size_t i = 0; i < inPoints.size(); i++)
	{
		const Point2D& point = inPoints[i];

		uint32_t vertexIndex = i * 4;

		outVertices[vertexIndex+0] =
		{
			{ point.x - halfWidth, point.y + halfWidth, 0.0f },
			{ 1.0f, 1.0f, 1.0f },
			{ 0.0f, 0.0f }
		};

		outVertices[vertexIndex+1] =
		{
			{ point.x + halfWidth, point.y + halfWidth, 0.0f },
			{ 1.0f, 1.0f, 1.0f },
			{ 1.0f, 0.0f }
		};

		outVertices[vertexIndex+2] =
		{
			{ point.x - halfWidth, point.y - halfWidth, 0.0f },
			{ 1.0f, 1.0f, 1.0f },
			{ 0.0f, 1.0f }
		};

		outVertices[vertexIndex+3] =
		{
			{ point.x + halfWidth, point.y - halfWidth, 0.0f },
			{ 1.0f, 1.0f, 1.0f },
			{ 1.0f, 1.0f }
		};

		outIndices[indexCount++] = vertexIndex;
		outIndices[indexCount++] = vertexIndex + 1;
		outIndices[indexCount++] = vertexIndex + 2;
		outIndices[indexCount++] = vertexIndex + 2;
		outIndices[indexCount++] = vertexIndex + 1;
		outIndices[indexCount++] = vertexIndex + 3;
	}
}

int main()
{
	std::vector<Point2D> points =
	{
		{ 0, 0 },
		{ 1, 1 },
		{ 2, 2 },
		{ 3, 3 },
	};

	std::vector<ModelVertex> vertices;
	std::vector<uint32_t> indices;
	PackPoints(points, vertices, indices);

	glibby::Window::Init();
	glibby::Renderer renderer;

	//Material* material = renderer->AddMaterial();
	renderer.AddModelByVertices(vertices, indices);
	//renderer.AddModelByFile("../../resources/models/viking_room.obj");
	renderer.Run();

	glibby::Window::Terminate();
}