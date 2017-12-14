### VERTEX ###

#version 430 core

uniform mat4 model_transform;
uniform mat4 vp_transform;
uniform vec3 upvec;
uniform vec3 skylight;
uniform vec3 groundlight;

layout(location=0) in vec4 position;
layout(location=2) in vec3 normal;

out vec4 ambientlight;

float saturate(float val){
    return min(1,max(0,val));
}

void main(){
    gl_Position = vp_transform*model_transform*position;
    vec3 transformed_normal = (model_transform*vec4(normal,1)).xyz;

    float sky = saturate(0.4f + 0.5f*dot(upvec, transformed_normal));
    float ground = saturate(0.1f + 0.5f*dot(-upvec, transformed_normal));
    ambientlight = sky*vec4(skylight,1) + ground*vec4(groundlight,1);
}

### FRAGMENT ###

#version 430 core

uniform vec4 obj_colour;

in vec4 ambientlight;
out vec4 frag_colour;

void main(){
    frag_colour = obj_colour*0.9f*ambientlight;
}
