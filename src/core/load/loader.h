#pragma once

#include <vector>
#include <memory>
#include "graphics/mesh.h"
#include "graphics/model.h"
#include "graphics/material.h"

using std::vector;
using std::shared_ptr;

class MeshLoader{
public:
    shared_ptr<Mesh> load(vector<float>& positions, vector<float>& texcoords, vector<float>& normals, vector<int>& indices);
    void unload(shared_ptr<Mesh>);
    ~MeshLoader();

private:
    GLuint storeDataInAttributeList(int index, int attr_size, vector<float>& data );
    GLuint createAndBindIndiceBuffer(vector<int>& indices);
};


namespace Load {
    vector<shared_ptr<Mesh>> obj_mesh(MeshLoader& loader, const std::string& file);
    vector<shared_ptr<Model>> obj_model(MeshLoader& loader, const std::string& file);
}
