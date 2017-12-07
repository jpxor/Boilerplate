
#include "util/fileutil.h"
#include "graphics/shader/BasicShader.h"
#include <iostream>

BasicShader::BasicShader(){
    auto sources = FileUtil::map_file("../res/shaders/basic.shader", {"### VERTEX ###","### FRAGMENT ###"});
    init(sources["### VERTEX ###"], sources["### FRAGMENT ###"]);
} 

BasicShader::~BasicShader(){ 
    ;;
} 