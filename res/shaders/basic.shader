### VERTEX ###

#version 400 core

layout(location=0) in vec4 position;

void main(){
    gl_Position = position;
}

### FRAGMENT ###

#version 400 core

uniform vec4 input_colour;
out vec4 frag_colour;

void main(){
    frag_colour = input_colour;
}
