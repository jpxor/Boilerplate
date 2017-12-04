#pragma once

#include <string>
#include <iostream>

#include "math/test_vec.h"
#include "math/test_matrix.h"

namespace bptest{

    void run(){
        bool pass = true;

        pass &= test_vec();
        pass &= test_matrix();

        std::string result = (pass)? "True" : "False";
        std::cout << "All tests pass: " << result << std::endl;
    } 

}