#pragma once

#include <vector>

struct GLFWwindow;

namespace glibby
{
    class Window 
    {
    public:
        Window(int width, int height, const char* name);
        virtual ~Window();
    
        Window(Window const&) = delete;
        Window& operator=(Window other) = delete;
    
        bool ShouldClose() const;
        void SwapBuffers() const;
        void PollEvents() const;
        void WaitEvents() const;

        uint32_t CreateVulkanWindowSurface(void* instance, void* surface) const;
        void GetFramebufferSize(int* width, int* height) const;
        void SetFramebufferResizeCallback(void(*callback)(int, int)) const;
    
        static void Init();
        static void Terminate();
        static void GetRequiredWindowExtensions(const char**& windowExtensions, uint32_t& windowExtensionCount);
    private:
        GLFWwindow* window;
    };
}