#include "glibby/renderer/vulkan_engine/VulkanEngine.h"
#include "glibby/renderer/Window.h"

void LoadScene(VulkanEngine& engine) {
	// Models
	Mesh triangleMesh;
	triangleMesh._vertices.resize(3);
	triangleMesh._vertices[0].position = { 1.f, 1.f, 0.0f };
	triangleMesh._vertices[1].position = {-1.f, 1.f, 0.0f };
	triangleMesh._vertices[2].position = { 0.f,-1.f, 0.0f };
	triangleMesh._vertices[0].color = { 0.f, 1.f, 0.0f }; //pure green
	triangleMesh._vertices[1].color = { 0.f, 1.f, 0.0f }; //pure green
	triangleMesh._vertices[2].color = { 0.f, 1.f, 0.0f }; //pure green

	Mesh monkeyMesh;
	monkeyMesh.load_from_obj("assets/monkey_smooth.obj");

	engine.upload_mesh(triangleMesh);
	engine.upload_mesh(monkeyMesh);

	engine._meshes["triangle"] = triangleMesh;
	engine._meshes["monkey"] = monkeyMesh;

	RenderObject monkey;
	monkey.mesh = engine.get_mesh("monkey");
	monkey.material = engine.get_material("defaultmesh");
	monkey.transformMatrix = glm::translate(glm::vec3(0, 2.0, 0));

	engine._renderables.push_back(monkey);

	for (int x = -20; x <= 20; x++) {
		for (int y = -20; y <= 20; y++) {
			RenderObject tri;
			tri.mesh = engine.get_mesh("triangle");
			tri.material = engine.get_material("defaultmesh");
			glm::mat4 translation = glm::translate(glm::mat4{ 1.0 }, glm::vec3(x, 0, y));
			glm::mat4 scale = glm::scale(glm::mat4{ 1.0 }, glm::vec3(0.2, 0.2, 0.2));
			tri.transformMatrix = translation * scale;
			engine._renderables.push_back(tri);
		}
	}
}

int main()
{
	Window::Init();

    VulkanEngine engine;
	engine.init();
	LoadScene(engine);
	engine.run();
	engine.cleanup();

	Window::Terminate();

	return 0;
}