
#include <string>
#include <iostream>
#include "graphics/shader/shader.h"

void Shader::init(std::string &vs_source, std::string &fs_source){
    vertex_shader = load_shader(vs_source, GL_VERTEX_SHADER);
    fragment_shader = load_shader(fs_source, GL_FRAGMENT_SHADER);

    program = glCreateProgram();
    glAttachShader(program, vertex_shader);
    glAttachShader(program, fragment_shader);

    //virtual function to be implemented in concrete shader
    bindAttributes();

    glLinkProgram(program);

    //dev only, is expensive operation
    glValidateProgram(program);

    //virtual function to be implemented in concrete shader
    getAllUniformLocations();
}

unsigned int Shader::load_shader(std::string &source, unsigned int shader_type){
    const char *raw_source = source.c_str();
 
    unsigned int shader = glCreateShader(shader_type);
    glShaderSource(shader, 1, &raw_source, NULL);
    glCompileShader(shader);

    int vertex_compiled;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &vertex_compiled);
    if (vertex_compiled != GL_TRUE)
    { 
        int log_length = 0;
        char message[1024];
        glGetShaderInfoLog(shader, 1024, &log_length, message);
        std::cout << message;
    }
    return shader;
}

int Shader::getUniformLocation(std::string uniform_name){
    const char *raw_name = uniform_name.c_str();
    return glGetUniformLocation(program, raw_name);
}

void Shader::bind_attr(unsigned int index, std::string attr_name){
    const char *raw_name = attr_name.c_str();
    glBindAttribLocation(program, index, raw_name);
}

Shader::~Shader(){
    stop();
    glDetachShader(program, vertex_shader);
    glDetachShader(program, fragment_shader);

    glDeleteShader(vertex_shader);
    glDeleteShader(fragment_shader);
    glDeleteProgram(program);
}
