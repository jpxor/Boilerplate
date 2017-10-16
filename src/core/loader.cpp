
#include <glewstatic.h>
#include <vector>
#include <memory>
#include <loader.h>

using std::vector;
using std::unique_ptr;
using std::make_unique;

//private constants
const int VERTPOS_DATA = 0;
const int TEXCOOR_DATA = 1;
const int NORMALS_DATA = 2;

//for cleanup
vector<GLuint> vaolist = {};
vector<GLuint> vbolist = {};
//map[mesh]->{vbolists}
 
unique_ptr<Mesh> MeshLoader::load(vector<float>& positions, vector<float>& texcoords, vector<float>& normals, vector<int>& indices){
    GLuint vao;
    glGenVertexArrays(1, &vao);
    vaolist.push_back(vao);

    glBindVertexArray(vao);
    GLuint vbo_indx = createAndBindIndiceBuffer(indices);
    GLuint vbo_vert = storeDataInAttributeList( VERTPOS_DATA, 3, positions );
    GLuint vbo_text = storeDataInAttributeList( TEXCOOR_DATA, 2, texcoords );
    GLuint vbo_norm = storeDataInAttributeList( NORMALS_DATA, 3, normals );
    glBindVertexArray(0);//unbind

    // Mesh* mesh = new Mesh(vao, indices.size() );
    auto mesh = make_unique<Mesh>(vao, indices.size() ); // C++14
    return mesh;
}

GLuint MeshLoader::createAndBindIndiceBuffer(vector<int>& indices){
    int* rawdata = &indices[0]; 
    size_t bytesize = sizeof(int) * indices.size();

    GLuint vbo;
    glGenBuffers(1, &vbo);
    vbolist.push_back(vbo);

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
    vbolist.push_back(vbo);

    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, bytesize, rawdata, GL_STATIC_DRAW);
    glVertexAttribPointer(idx, attr_size, GL_FLOAT, GL_FALSE, 0, 0); // stride==0 mean tightly packed.
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    return vbo;
}


void MeshLoader::unload(unique_ptr<Mesh> mesh){
    //find vbo's in map
}


MeshLoader::~MeshLoader(){
    for(GLuint vao : vaolist){
        glDeleteVertexArrays( 1, &vao );
    }
    for(GLuint vbo : vbolist){
        glDeleteBuffers(1, &vbo);
    }
}
