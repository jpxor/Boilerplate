
#include <mesh.h>

Mesh::Mesh(GLuint vaoid, size_t vcount) : vao_id(vaoid), vert_count(vcount) {;;}
GLuint Mesh::vao(){return vao_id;}
size_t Mesh::vcount(){return vert_count;}