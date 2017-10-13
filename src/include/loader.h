#pragma once

#include <vector>
#include "mesh.h"

using std::vector;

class Loader{
public:
    Mesh* loadToVAO(vector<float>& positions);

private:
    int createVAO();
    void unbindVAO();
    void storeDataInAttributeList(int index, vector<float>& data );
    ~Loader();
};

