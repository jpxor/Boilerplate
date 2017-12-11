### VERTEX ###

#version 430 core

uniform mat4 transform;
uniform vec3 upvec;
uniform vec3 skylight;
uniform vec3 groundlight;

layout(location=0) in vec4 position;
layout(location=2) in vec3 normal;

out vec4 ambientlight;


void main(){
    gl_Position = transform*position;
    float sky = max(0.2f,dot(upvec, normal));
    float ground = max(0.1f,dot(-upvec, normal));
    ambientlight = 1.5*sky*vec4(skylight,1) + ground*vec4(groundlight,1);
}

### FRAGMENT ###

#version 430 core

uniform vec4 obj_colour;

in vec4 ambientlight;
out vec4 frag_colour;

void main(){
    frag_colour = obj_colour * 0.5f*ambientlight;
}
