#include "window.h"

#define GLFW_INCLUDE_VULKAN
#include "GLFW/glfw3.h"

namespace glibby
{
    static void (*frameBufferResizeCallback)(int, int) = nullptr;

    static void FramebufferResizeCallback(GLFWwindow* window, int width, int height)
    {
        if (frameBufferResizeCallback)
        {
            frameBufferResizeCallback(width, height);
        }
    }

    static void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
    {
        if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        {
            glfwSetWindowShouldClose(window, GLFW_TRUE);
        }
    }

    Window::Window(int width, int height, const char* name)
    { 
        glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API); // disable automatic opengl context. vulkan gang!
        glfwWindowHint(GLFW_RESIZABLE, GLFW_TRUE);

        this->window = glfwCreateWindow(width, height, name, NULL, NULL);
        glfwSetKeyCallback(this->window, KeyCallback);
        glfwSetFramebufferSizeCallback(window, FramebufferResizeCallback);
        glfwMakeContextCurrent(this->window);
        glfwSwapInterval(1);
    }
    
    Window::~Window()
    {
        glfwDestroyWindow(this->window);
    }

    bool Window::ShouldClose() const
    {
        return glfwWindowShouldClose(this->window);
    }

    void Window::SwapBuffers() const
    {
        glfwSwapBuffers(this->window);
    }

    void Window::PollEvents() const
    {
        glfwPollEvents();
    }

    void Window::WaitEvents() const
    {
        glfwWaitEvents();
    }

    uint32_t Window::CreateVulkanWindowSurface(void* instance, void* surface) const
    {
        return glfwCreateWindowSurface((VkInstance)instance, window, nullptr, (VkSurfaceKHR*)surface);
    }

    void Window::GetFramebufferSize(int* width, int* height) const
    {
        glfwGetFramebufferSize(window, width, height);
    }

    void Window::SetFramebufferResizeCallback(void(*callback)(int, int)) const
    {
        frameBufferResizeCallback = callback;
    }
    
    void Window::Init()
    {
        glfwInit();
    }
    
    void Window::Terminate()
    {
        glfwTerminate();
    }

    void Window::GetRequiredWindowExtensions(const char**& windowExtensions, uint32_t& windowExtensionCount)
    {
        windowExtensions = glfwGetRequiredInstanceExtensions(&windowExtensionCount);
    }
}
