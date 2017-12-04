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
    std::cout << "  > mat4 passed: " << result << std::endl;
    return pass;
}



static bool test_matrix(){
    bool pass = true;
    
    std::cout << "test_matrix: matrix math" << std::endl;
    pass &= test_mat4();

    std::string result = (pass)? "True" : "False";
    std::cout << "> test_matrix passed: " << result << std::endl;
    return pass;
}