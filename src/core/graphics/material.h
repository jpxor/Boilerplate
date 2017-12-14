#pragma once

#include <memory>
#include "graphics/texture.h"

class Material{
    std::vector<std::shared_ptr<Texture>> textures;
};