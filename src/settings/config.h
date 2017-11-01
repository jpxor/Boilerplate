#pragma once

#include <string>
#include <memory>

namespace Settings
{
    enum class GraphicsAPI{
        OpenGL,
        DirectX12
    };

    struct Configuration
    {
        bool fullscreen;
        int width;
        int height;
        GraphicsAPI graphics_api;
    };

    Configuration load();
}