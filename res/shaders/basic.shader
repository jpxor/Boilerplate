### VERTEX ###

#version 430 core

uniform mat4 transform;
layout(location=0) in vec4 position;


void main(){
    gl_Position = transform*position;
}

### FRAGMENT ###

#version 430 core

uniform vec4 input_colour;
out vec4 frag_colour;

void main(){
    frag_colour = input_colour;
}
