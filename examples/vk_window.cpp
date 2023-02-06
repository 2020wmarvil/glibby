#include "glibby/renderer/renderer.h"
#include "glibby/renderer/window.h"
#include "glibby/renderer/model.h"

int main()
{
	glibby::Window::Init();
	glibby::Renderer renderer;
	
	renderer.AddModelByFile("../../resources/models/viking_room.obj");
	renderer.Run();

	glibby::Window::Terminate();
}