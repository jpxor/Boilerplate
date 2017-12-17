#pragma once

#include "graphics/shader/shader.h"


class WaveShader : public Shader {
    public:
    WaveShader();
    ~WaveShader();
    
    void load_colour(float r, float g, float b, float a){
        load("obj_colour", vec4(r,g,b,a) );  
    }

    void load_time(float t){
        load("time", t );  
    }

    void load_score(float score){
        load("score", score );  
    }

    void load_spike(float x){
        load("spike", x );  
    }

    void load_scale(float s){
        load("scale", s );  
    }

    void load_total_length(float s){
        load("total_length", s );  
    }

    void load_params(float a, float b, float c, float d){
        load("wave_params", vec4(a,b,c,d) );  
    }

    void load_skylight(float r, float g, float b){
        load("skylight", vec3(r,g,b) );  
    }

    void load_groundlight(float r, float g, float b){
        load("groundlight", vec3(r,g,b) );  
    }

    void load_waveaxis(float x, float y, float z){
        load("wave_axis", vec3(x,y,z) );  
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