#pragma once

#include <string>
#include <iostream>
#include "math/vec.h"


#define update(status) \
    pass &= status; \
    result = (status)? "Pass" : "*FAIL*"; \
    std::cout << "    > " << #status << ": " << result << std::endl

using namespace bpm;


static bool test_vec3(){
    std::string result;

    std::cout << "  vec3 tests" << std::endl;
    bool pass = true;

    vec3 ux(1,0,0);
    vec3 uy(0,1,0);
    vec3 uz(0,0,1);

    bool equals = ( ux == vec3(1,0,0) );
    equals &= ( bpm::equals(uy, vec3(0,1,0)) );
    equals &= !( ux == uz );
    update(equals);

    bool addition = (ux+uy+uz == vec3(1,1,1));
    update(addition);

    bool subtraction = (ux-(uy-uz) == vec3(1,-1,1));
    update(subtraction);

    bool scale = ( 4*ux + uy*2 + 0*uz == vec3(4,2,0));
    update(scale);

    bool dot = ( bpm::dot(ux, uy) == 0 );
    dot &= ( bpm::dot( vec3(1,2,3), vec3(4,5,6) ) == 32 );
    update(dot);

    bool cross = (  ux*uy == uz );
    cross &= ( vec3(1,2,3) * vec3(4,5,6) == vec3(-3,6,-3) );
    update(cross);

    vec3 m(ux);
    m += uy;
    m-= uz;
    m*= 2;
    bool mutable_ops = (  m == vec3(2,2,-2) );
    update(mutable_ops);

    result = (pass)? "True" : "False";
    std::cout << "  > vec3 passed: " << result << std::endl;
    return pass;
}



static bool test_vec(){
    bool pass = true;
    
    std::cout << "test_vec: vector math" << std::endl;
    //pass &= test_vec2();
    pass &= test_vec3();
    //pass &= test_vec3d();
    //pass &= test_vec4();

    std::string result = (pass)? "True" : "False";
    std::cout << "> test_vec passed: " << result << std::endl;
    return pass;
}


