#pragma once

#include <vector>

struct GLFWwindow;

namespace glibby
{
    struct InputState;

    class Window 
    {
    public:
        Window(int width, int height, const char* name, InputState* inputState);
        virtual ~Window();
    
        Window(Window const&) = delete;
        Window& operator=(Window other) = delete;
    
        void ProcessInput() const;
        bool ShouldClose() const;
        void SwapBuffers() const;
        void PollEvents() const;
        void WaitEvents() const;

        uint32_t CreateVulkanWindowSurface(void* instance, void* surface) const;
        void GetFramebufferSize(int* width, int* height) const;
        void SetFramebufferResizeCallback(void(*callback)(int, int)) const;
        InputState* GetInputState() { return inputState; }
        double GetTime() const;
    
        static void Init();
        static void Terminate();
        static void GetRequiredWindowExtensions(const char**& windowExtensions, uint32_t& windowExtensionCount);
    private:
        GLFWwindow* window;
        InputState* inputState;
    };
}