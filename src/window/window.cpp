
#include "window/window.h"

namespace Window{

    Window::Instance init(std::string program_name, Settings::Configuration config){    
        if ( !glfwInit() ) {
            throw std::runtime_error("Failed to initialize GLFW library");
        }
        auto window = glfwCreateWindow(config.width, config.height, program_name.c_str(), NULL, NULL);
        if (!window){
            glfwTerminate();
            throw std::invalid_argument("Failed to create window");
        }
        if( config.fullscreen ){
            glfwSetWindowMonitor(window, glfwGetPrimaryMonitor(), 0, 0, config.width, config.height, 0);
        }
        glfwMakeContextCurrent(window);
        return Window::Instance{window};
    }
    
}