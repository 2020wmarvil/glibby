#include "glibby/renderer/renderer.h"

#include <iostream> 

int main()
{
	uint32_t extensionsCount = glibby::GetInstanceExtensionsCount();
	std::cout << "Available VK Instance Extensions: " << extensionsCount << std::endl;

	glibby::Window::Init();
	glibby::Window window(800, 600, "Demo Window");
	window.Run();
	glibby::Window::Terminate();
} 