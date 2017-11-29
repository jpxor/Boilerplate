### VERTEX ### 

#version 400 core

layout(location=0) in vec4 position;

void main(){
    gl_Position = position;
}

### FRAGMENT ###

#version 400 core
out vec4 color;

void main(){
    color = vec4(0.75, 0.75, 0.75, 1);
}