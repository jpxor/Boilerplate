
#pragma once

#include <GLFW/glfw3.h>
#include "settings/config.h"

namespace Window{

    struct Instance{
        GLFWwindow* id;

        inline bool shouldClose(){
            return glfwWindowShouldClose(id);
        }
    
        inline void close() const { //must be const to use call from captured window instance in lambda function??
            glfwSetWindowShouldClose(id, GLFW_TRUE);
        }

    };

    inline void terminate(){
        glfwTerminate(); 
    }

    Window::Instance init(std::string program_name, Settings::Configuration config);
    
}
