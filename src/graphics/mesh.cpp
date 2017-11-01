
#include <graphics/mesh.h>

Mesh::Mesh(GLuint vaoid, size_t vcount) : vao_id(vaoid), vert_count((GLuint)vcount) {;;}
GLuint Mesh::vao(){return vao_id;} 
GLuint Mesh::vcount(){return (GLuint)vert_count;}
