#include "renderer.h"

#define GLFW_INCLUDE_VULKAN
#include "GLFW/glfw3.h"

namespace glibby
{
    static void KeyCallback(GLFWwindow* Window, int Key, int Scancode, int Action, int Mods)
    {
        if (Key == GLFW_KEY_ESCAPE && Action == GLFW_PRESS)
        {
            glfwSetWindowShouldClose(Window, GLFW_TRUE);
        }
    }

    Window::Window(int width, int height, const char* name)
    { 
        this->window = glfwCreateWindow(width, height, name, NULL, NULL);
        glfwSetKeyCallback(this->window, KeyCallback);
        glfwMakeContextCurrent(this->window);
        glfwSwapInterval(1);
    }
    
    Window::~Window()
    {
        glfwDestroyWindow(this->window);
    }
    
    void Window::Run()
    {
        while (!glfwWindowShouldClose(this->window))
        {
            int Width, Height;
            glfwGetFramebufferSize(this->window, &Width, &Height);
            glfwSwapBuffers(this->window);
            glfwPollEvents();
        }
    }
    
    void Window::Init()
    {
        glfwInit();
        glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API); // disable automatic opengl context. vulkan gang!
        glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
    }
    
    void Window::Terminate()
    {
        glfwTerminate();
    }

	uint32_t GetInstanceExtensionsCount()
	{
		uint32_t count;
		vkEnumerateInstanceExtensionProperties(nullptr, &count, nullptr);
		return count;
	}
}
