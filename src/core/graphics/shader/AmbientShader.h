#pragma once

#include "graphics/shader/shader.h"


class AmbientShader : public Shader {
    public:
    AmbientShader();
    ~AmbientShader();
    
    void load_colour(float r, float g, float b){
        load("obj_colour", vec4(r,g,b,1) );  
    }

    void load_skylight(float r, float g, float b){
        load("skylight", vec3(r,g,b) );  
    }

    void load_groundlight(float r, float g, float b){
        load("groundlight", vec3(r,g,b) );  
    }

    void load_upvec(float x, float y, float z){
        load("upvec", vec3(x,y,z) );  
    }

    void load_transform_model(mat4 matrix){
        load("model_transform", matrix);  
    }

    void load_transform_vp(mat4 matrix){
        load("vp_transform", matrix);  
    }
    
    private: 

    void bindAttributes(){
        bind_attr(POSITION_ATTR, "position");
        bind_attr(TEXTURE_ATTR, "texcoord");
        bind_attr(NORMAL_ATTR, "normal");
    }

};