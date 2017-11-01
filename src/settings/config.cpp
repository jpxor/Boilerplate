
#include "settings/config.h" 

namespace Settings
{
    Configuration load()
    { 
        Configuration config = {};
        config.fullscreen = false;
        config.width = 960;
        config.height = 540;
        config.graphics_api = Settings::GraphicsAPI::OpenGL;
        return config;
    }
}