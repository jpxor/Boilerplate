#pragma once

#include <vector>
#include <memory>
#include "graphics/mesh.h"

using std::vector;
using std::unique_ptr;

class MeshLoader{
public:
    unique_ptr<Mesh> load(vector<float>& positions, vector<float>& texcoords, vector<float>& normals, vector<int>& indices);
    void unload(unique_ptr<Mesh>);
    ~MeshLoader();

private:
    GLuint storeDataInAttributeList(int index, int attr_size, vector<float>& data );
    GLuint createAndBindIndiceBuffer(vector<int>& indices);
};


namespace Load {
    unique_ptr<Mesh> OBJ(MeshLoader& loader, const std::string& file);
}
