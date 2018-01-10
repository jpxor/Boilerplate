
#include "graphics/material.h"
#include "util/fileutil.h"
#include <unordered_map>

MaterialShader::MaterialShader(){;;}
MaterialShader::~MaterialShader(){;;}

void MaterialShader::load_diffuse_color(const bpm::vec3& diffuse_color) {
    load("diffuse_color", diffuse_color);
}

void MaterialShader::load_specular_color(const bpm::vec3& specular_color) {
    load("specular_color", specular_color);
}

void MaterialShader::load_specular_exponent(float specular_exponent) {
    load("specular_exponent", specular_exponent);
}

void MaterialShader::load_transparency(float transparency) {
    load("transparency", transparency);
}

void MaterialShader::init(){
    auto sources = FileUtil::map_file("../res/shaders/material.shader", {"### VERTEX ###","### FRAGMENT ###"});
    Shader::init(sources["### VERTEX ###"], sources["### FRAGMENT ###"]);
}


static MaterialShader basic_material_shader;

BasicMaterial::BasicMaterial(const bpm::vec3& diffuse, const bpm::vec3& specular, float spec_exp, float _transparency)
    :   diffuse_color(diffuse), 
        specular_color(specular), 
        specular_exponent(spec_exp),
        transparency(_transparency) 
    {
        basic_material_shader.init();
    }

void BasicMaterial::apply(Shader& shader){
    shader.load("diffuse_color", diffuse_color);
    shader.load("specular_color", specular_color);
    shader.load("specular_exponent", specular_exponent);
    shader.load("transparency", transparency);
}

std::string BasicMaterial::shader_name(){
    return "basic_material_shader";
}

MaterialShader& BasicMaterial::shader(){
    return basic_material_shader;
}


