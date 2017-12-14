
#include "settings/config.h" 

namespace Settings
{
    Configuration load()
    { 
        Configuration config = {};
        config.fullscreen = false;
        config.width = 960*1.6;
        config.height = 540*1.6;
        config.graphics_api = Settings::GraphicsAPI::OpenGL;
        return config;
    }
}