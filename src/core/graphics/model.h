#pragma once

#include <memory>
#include "graphics/mesh.h"
#include "graphics/material.h"

class Model{
    std::shared_ptr<Mesh> mesh;
    std::shared_ptr<Material> material;
};