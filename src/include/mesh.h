#pragma once

#include <glewstatic.h>

class Mesh {
private: 
    GLuint vao_id;
    size_t vert_count;

public: 
    Mesh(GLuint vaoid, size_t vcount);
    GLuint vao();
    GLuint vcount();
};

