#pragma once

#include <string>
#include <iostream>
#include "math/matrix.h"

#define update(status) \
    pass &= status; \
    result = (status)? "Pass" : "*FAIL*"; \
    std::cout << "    > " << #status << ": " << result << std::endl

using namespace bpm;



static bool test_mat4(){
    std::string result;

    std::cout << "  mat4 tests" << std::endl;
    bool pass = true;

    mat4 I,S,T,R;
    make_identity(I);
    make_scale(S, vec3(2, 1, 2.0/3.0));
    make_translation(T, vec3(-1,-1,-1));
    make_rotation(R, vec3(0,0,1), 1.5708f);

    bool equals = ( I == I );
    equals &= ( R == R );
    equals &= !( I == R );
    update(equals);

    vec3 v(1,2,3);

    bool identity = ( v == I*v );
    update(identity);

    bool scale = ( vec3(2,2,2) == S*v );
    update(scale);

    bool translation = ( vec3(0,1,2) == T*v );
    update(translation);

    bool rotation = ( bpm::equals(vec3(-2,1,3), R*v, 1e-4) );
    update(rotation);

    vec3 v0 = (R*(T*(S*(I*v))));
    vec3 v1 = (R*T*S*I)*v;
    bool matrix_multiply = ( v0 == v1 );
    update(matrix_multiply);

    result = (pass)? "True" : "False";
    std::cout << "  > mat4 passed: " << result << std::endl << std::endl;
    return pass;
}


static void printmat(const mat4& m){
    for(int r = 0; r < 4; ++r){
        for(int c = 0; c < 4; ++c){
            std::cout << m.e[c][r] << " ";
        }
        std::cout << std::endl;
    }
}


static bool test_mat4_utils(){
    std::string result;

    std::cout << "  mat4_utils tests" << std::endl;
    bool pass = true;

    float x=3,y=7,z=1;

    mat4 V,V2;
    make_view(V, vec3(x,y,z), vec3(1,0,0), vec3(0,1,0));
    make_view_lookat(V2, vec3(x,y,z), vec3(x+10,y,z), vec3(0,1,0));

    bool view = true;
    view &= (-x == V.e[3][0]) && (-y == V.e[3][1]) && (-z == V.e[3][2]);
    view &= ( V*vec3(x+2,y,z) == vec3(2,0,0) );///not working as expected. understanding fail? 
    view &= V==V2;
    update(view);

    vec3 r = V*vec3(x+2,y,z);
    std::cout << r.x << " " << r.y << " " << r.z << std::endl;

    mat4 P;
    make_perspective(P,1.25,1,1,1000);
    // printmat(P);

    result = (pass)? "True" : "False";
    std::cout << "  > mat4 utils passed: " << result << std::endl << std::endl;
    return pass;
}



static bool test_matrix(){
    bool pass = true;
    
    std::cout << "test_matrix: matrix math" << std::endl;
    pass &= test_mat4();

    std::cout << "test_matrix: utils" << std::endl;
    pass &= test_mat4_utils();

    std::string result = (pass)? "True" : "False";
    std::cout << "> test_matrix passed: " << result << std::endl << std::endl;
    return pass;
}