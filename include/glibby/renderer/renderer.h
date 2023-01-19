#pragma once

#include <stdint.h>

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
    
        void Run();
    
        static void Init();
        static void Terminate();
    private:
        GLFWwindow* window;
    };

	uint32_t GetInstanceExtensionsCount();
}
