#include "Window.h"

#include <iostream>

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

bool Window::ShouldClose()
{
    return glfwWindowShouldClose(window);
}

void Window::SwapBuffers()
{
    glfwSwapBuffers(window);
}

void Window::PollEvents()
{
    glfwPollEvents();
}

void Window::CreateSurface(VkInstance instance, const VkAllocationCallbacks *allocationCallbacks, VkSurfaceKHR *surface) const
{
    if (glfwCreateWindowSurface(instance, window, nullptr, surface) != VK_SUCCESS)
    {
        std::cerr << "Failed to create surface from window\n";
    }
}

void Window::GetWindowSize(int& outWidth, int& outHeight)
{
    glfwGetWindowSize(window, &outWidth, &outHeight);
}

void Window::GetFramebufferSize(int& outWidth, int& outHeight)
{
    glfwGetFramebufferSize(window, &outWidth, &outHeight);
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
