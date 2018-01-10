
#include "graphics/glewstatic.h"
#include <load/loader.h>

#include <memory>
#include <vector>
#include <unordered_map>

using std::vector;
using std::shared_ptr;
using std::make_shared;
using std::unordered_map;

//private constants
const int VERTPOS_DATA = 0;
const int TEXCOOR_DATA = 1;
const int NORMALS_DATA = 2;

//for cleanup
static unordered_map<GLuint, vector<GLuint>> cleanup_map = {};
 

shared_ptr<Mesh> MeshLoader::load(vector<float>& positions, vector<float>& texcoords, vector<float>& normals, vector<int>& indices){
    GLuint vao;
    glGenVertexArrays(1, &vao);

    glBindVertexArray(vao);
    GLuint vbo_indx = createAndBindIndiceBuffer(indices);
    GLuint vbo_vert = storeDataInAttributeList( VERTPOS_DATA, 3, positions );
    GLuint vbo_text = storeDataInAttributeList( TEXCOOR_DATA, 2, texcoords );
    GLuint vbo_norm = storeDataInAttributeList( NORMALS_DATA, 3, normals );
    glBindVertexArray(0);//unbind

    cleanup_map.emplace(vao, vector<GLuint>() );
    auto vbos = cleanup_map[vao];

    vbos.push_back(vbo_indx);
    vbos.push_back(vbo_vert);
    vbos.push_back(vbo_text);
    vbos.push_back(vbo_norm);
    
    auto mesh = make_shared<Mesh>(vao, indices.size() ); // C++14
    return mesh;
}


GLuint MeshLoader::createAndBindIndiceBuffer(vector<int>& indices){
    int* rawdata = &indices[0]; 
    size_t bytesize = sizeof(int) * indices.size();

    GLuint vbo;
    glGenBuffers(1, &vbo);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vbo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, bytesize, rawdata, GL_STATIC_DRAW);
    //no unbind!??

    return vbo;
}
 

GLuint MeshLoader::storeDataInAttributeList(int idx, int attr_size, vector<float>& data ){
    float* rawdata = &data[0]; 
    size_t bytesize = sizeof(float) * data.size();

    GLuint vbo;
    glGenBuffers(1, &vbo);

    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, bytesize, rawdata, GL_STATIC_DRAW);
    glVertexAttribPointer(idx, attr_size, GL_FLOAT, GL_FALSE, 0, 0); // stride==0 mean tightly packed.
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    return vbo;
}


void MeshLoader::unload(shared_ptr<Mesh> mesh){
    GLuint vao = mesh->vao();
    glDeleteVertexArrays( 1, &vao );

    auto vbos = cleanup_map[vao];
    cleanup_map.erase(vao);
    
    for(const GLuint& vbo : vbos){
        glDeleteBuffers(1, &vbo);
    }
}


MeshLoader::~MeshLoader(){
    for(const auto& kv : cleanup_map){
        GLuint vao = kv.first;
        glDeleteVertexArrays( 1, &vao );

        auto vbos = kv.second;
        for(const GLuint& vbo : vbos){
            glDeleteBuffers(1, &vbo);
        }
    }
}
