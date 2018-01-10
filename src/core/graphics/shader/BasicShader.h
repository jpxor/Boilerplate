
#pragma once

#include "graphics/shader/shader.h"

class BasicShader : public Shader {
public:
    BasicShader();
    ~BasicShader();
    
    
    void load_color(float r, float g, float b){
        load("input_colour", vec4(r,g,b,1) );  
    }

    void load_transform(mat4 matrix){
        load("transform", matrix);  
    }
    
private: 
    void bindAttributes(){
        bind_attr(POSITION_ATTR, "position");
        bind_attr(TEXTURE_ATTR, "texcoord");
        bind_attr(NORMAL_ATTR, "normal");
    }

};