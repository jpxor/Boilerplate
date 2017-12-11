
#include <iostream>
#include "graphics/ginit.h"
#include "graphics/glewstatic.h"

namespace Graphics{

    void init(Settings::Configuration conf){
        if( conf.graphics_api == Settings::GraphicsAPI::OpenGL ){
            GLenum err = glewInit();
            if (GLEW_OK != err)
            {
              std::cout << "Error: " << glewGetErrorString(err) << std::endl;
              throw std::runtime_error("Failed to initialize GLEW library");
            }
            glEnable(GL_CULL_FACE);
            glEnable(GL_DEPTH_TEST);
        }
        
    }
    
}