
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
        std::cout << message << std::endl;
        std::cout << source << std::endl;
    }
    return shader;
}

int Shader::getUniformLocation(std::string uniform_name) {
    int loc = locations[uniform_name];
    if( loc > 0 ){
        loc = loc-1;
    }
    else{
        loc = glGetUniformLocation(program, uniform_name.c_str());
        locations[uniform_name] = loc+1;
    }
    return loc;
}

void Shader::bind_attr(unsigned int index, std::string attr_name){
    const char *raw_name = attr_name.c_str();
    glBindAttribLocation(program, index, raw_name);
}


void Shader::load(std::string uniform, float val) {
    auto loc = getUniformLocation(uniform);
    glUniform1f(loc, val);
}

void Shader::load(std::string uniform, const vec2& vec) {
    auto loc = getUniformLocation(uniform);
    glUniform2f(loc, vec.x, vec.y);
}

void Shader::load(std::string uniform, const vec3& vec) {
    auto loc = getUniformLocation(uniform);
    glUniform3f(loc, vec.x, vec.y, vec.z);
}

void Shader::load(std::string uniform, const vec4& vec) {
    auto loc = getUniformLocation(uniform);
    glUniform4f(loc, vec.x, vec.y, vec.z, vec.w);
}

void Shader::load(std::string uniform, const mat4& matrix) {
    auto loc = getUniformLocation(uniform);
    glUniformMatrix4fv(loc, 1, GL_FALSE, (float*)matrix.e);
}

void Shader::load(std::string uniform, const std::vector<vec3>& vec_array){
    auto loc = getUniformLocation(uniform);
    glUniform3fv(loc, 3*vec_array.size(), &vec_array[0].x);
}
 
Shader::~Shader(){
    stop();
    glDetachShader(program, vertex_shader);
    glDetachShader(program, fragment_shader);

    glDeleteShader(vertex_shader);
    glDeleteShader(fragment_shader);
    glDeleteProgram(program);
}
