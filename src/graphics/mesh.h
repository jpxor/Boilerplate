#pragma once

#include "graphics/glewstatic.h"

class Mesh {
private:
    GLuint vao_id;
    GLuint vert_count;

public: 
    Mesh(GLuint vaoid, size_t vcount);
    GLuint vao();
    GLuint vcount();
};
