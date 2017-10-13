
#include <glewstatic.h>
#include <vector>

#include <loader.h>

using std::vector;

//private constants
const int VERTPOS_DATA = 0;

//for cleanup
vector<Mesh*> meshPointers = {};
vector<GLuint> vbolist = {};

Mesh* Loader::loadToVAO(vector<float>& positions){
    int vao = createVAO();

    storeDataInAttributeList( VERTPOS_DATA, positions );
    unbindVAO();

    Mesh* mesh = new Mesh(vao, positions.size() );
    meshPointers.push_back(mesh);
    return mesh;
}

int Loader::createVAO(){
    GLuint vao;
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);
    return vao;
}

void Loader::unbindVAO(){
    glBindVertexArray(0);
}

void Loader::storeDataInAttributeList(int index, vector<float>& data ){
    float* rawdata = &data[0]; 
    size_t bytesize = sizeof(float) * data.size();

    GLuint vbo;
    glGenBuffers(1, &vbo);
    vbolist.push_back(vbo);

    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, bytesize, rawdata, GL_STATIC_DRAW);
    glVertexAttribPointer(index, 3, GL_FLOAT, false, 0, 0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

Loader::~Loader(){
    for(Mesh* mesh : meshPointers){
        GLuint vao = mesh->vao();
        glDeleteVertexArrays( 1, &vao );
        delete mesh;
    }
    for(GLuint vbo : vbolist){
        glDeleteBuffers(1, &vbo);
    }
}