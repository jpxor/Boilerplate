#pragma once

#include <string>
#include "math/vec.h"
#include "graphics/shader/shader.h"

class Material{
public:
    virtual void apply(Shader&) = 0;
    virtual std::string shader_name() = 0;
};

class MaterialShader : public Shader {
public:
    MaterialShader();
    ~MaterialShader();
    void load_diffuse_color(const bpm::vec3& diffuse_color);
    void load_specular_color(const bpm::vec3& specular_color);
    void load_specular_exponent(float specular_exponent);
    void load_transparency(float transparency);
    void init();

private: 
    void bindAttributes(){
        bind_attr(POSITION_ATTR, "position");
        bind_attr(TEXTURE_ATTR, "texcoord");
        bind_attr(NORMAL_ATTR, "normal");
    }
};

class BasicMaterial : public Material {
public: 
    bpm::vec3 diffuse_color;
    bpm::vec3 specular_color;
    float specular_exponent;
    float transparency;

    BasicMaterial(const bpm::vec3& diffuse, const bpm::vec3& specular, float spec_exp, float _transparency);

    void apply(Shader& shader);
    std::string shader_name();
    MaterialShader& shader();
};

