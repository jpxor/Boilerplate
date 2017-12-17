### VERTEX ###

#version 430 core

uniform mat4 model_transform;
uniform mat4 vp_transform;
uniform vec3 wave_axis;
uniform vec4 wave_params;
uniform vec3 skylight;
uniform vec3 groundlight;
uniform float scale;
uniform float spike;
uniform float time;
uniform float score;
uniform float total_length;

layout(location=0) in vec4 position;
layout(location=2) in vec3 normal;

out vec4 ambientlight;

float saturate(float val){
    return min(1,max(0,val));
}

void main(){
    float a = wave_params.x;
    float b = wave_params.y;
    float c = wave_params.z;
    float d = wave_params.w;

    float yscale = scale / (1 + 0.5*abs(spike-position.x));
    float xpos = 10*3.14*position.x / total_length; 
    vec4 tempos = position;
    gl_Position = position;
    gl_Position.y = 
        yscale*0.5*sin(a*time + b*xpos) 
        + 0.25*cos(-c*time + d*xpos) 
        + score*sin(xpos/10)
        + 0.1*sin(5*time+xpos/5);
    gl_Position = vp_transform*model_transform*gl_Position;
}

### FRAGMENT ###

#version 430 core

uniform vec4 obj_colour;
out vec4 frag_colour;

void main(){
    frag_colour = obj_colour;
}
