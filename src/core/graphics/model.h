#pragma once

#include <memory>
#include "graphics/mesh.h"
#include "graphics/material.h"

class Model{
public:
    std::shared_ptr<Mesh> mesh;
    std::shared_ptr<Material> material;

    Model(std::shared_ptr<Mesh> _mesh, std::shared_ptr<Material> _material)
        : mesh(_mesh), material(_material) {;;}
};