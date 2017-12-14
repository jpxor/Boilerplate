#pragma once

#include <memory>
#include "graphics/model.h"
#include "graphics/shader/shader.h"

class Renderable{
    std::shared_ptr<Shader> shader;
    std::shared_ptr<Model> model;
    // std::shared_ptr<Material> material;
}; 


//shader --> shader renderer
//              list of renderables using this shader
//                  sorted by mesh and texture

class Renderer{

};

namespace Systems{
    void render(std::unordered_map<std::shared_ptr<Shader>,std::unordered_map<std::shared_ptr<Model>,Renderable>> renderables){

    }
} 