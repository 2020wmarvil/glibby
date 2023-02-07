#include "window.h"

#include "input.h"
#include "camera.h"

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

    static void MouseCallback(GLFWwindow* window, double xPos, double yPos)
    {
        Window* container = (Window*)glfwGetWindowUserPointer(window);
        
        if (container->GetInputState()->firstMouse)
        {
            container->GetInputState()->lastX = static_cast<float>(xPos);
            container->GetInputState()->lastY = static_cast<float>(yPos);
            container->GetInputState()->firstMouse = false;
        }
        
        container->GetInputState()->currentX = static_cast<float>(xPos);
        container->GetInputState()->currentY = static_cast<float>(yPos); 
    }

    Window::Window(int width, int height, const char* name, InputState* inputState)
        : inputState(inputState)
    { 
        glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API); // disable automatic opengl context. vulkan gang!
        glfwWindowHint(GLFW_RESIZABLE, GLFW_TRUE);

        this->window = glfwCreateWindow(width, height, name, NULL, NULL);
        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
        glfwSetFramebufferSizeCallback(window, FramebufferResizeCallback);
        glfwSetCursorPosCallback(window, MouseCallback);
        glfwSetWindowUserPointer(window, this);
        glfwMakeContextCurrent(this->window);
        glfwSwapInterval(1);
    }
    
    Window::~Window()
    {
        glfwDestroyWindow(this->window);
    }

    void Window::ProcessInput() const
    {
        if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        {
            glfwSetWindowShouldClose(window, true);
        }

        if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) inputState->bIsWPressed = true;
        else                                              inputState->bIsWPressed = false;

        if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) inputState->bIsSPressed = true;
        else                                              inputState->bIsSPressed = false;
        
        if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) inputState->bIsAPressed = true;
        else                                              inputState->bIsAPressed = false;

        if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) inputState->bIsDPressed = true;
        else                                              inputState->bIsDPressed = false;

        if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS) inputState->bIsEPressed = true;
        else                                              inputState->bIsEPressed = false;

        if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS) inputState->bIsQPressed = true;
        else                                              inputState->bIsQPressed = false;
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

    double Window::GetTime() const
    {
        return glfwGetTime();
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
