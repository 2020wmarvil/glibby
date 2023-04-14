#include "Window.h"

#include "input.h"

#include <iostream>

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

Window::Window(int width, int height, const char* name)
{ 
    this->window = glfwCreateWindow(width, height, name, NULL, NULL);
    //glfwSetKeyCallback(this->window, KeyCallback);
    //glfwMakeContextCurrent(this->window);
    //glfwSwapInterval(1);

    this->inputState = std::make_unique<InputState>();

    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
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

double Window::GetTime() const
{
    return glfwGetTime();
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
