#pragma once

#include "window.h"

#include <memory>
#include <stdint.h>

namespace glibby {
    class Renderer
    {
    public:
        Renderer();
        virtual ~Renderer();
    
        Renderer(Renderer const&) = delete;
        Renderer& operator=(Renderer other) = delete;

        void Run();

    private:
        std::unique_ptr<Window> window;
    };
}
