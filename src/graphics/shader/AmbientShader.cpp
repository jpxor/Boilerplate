
#include "util/fileutil.h"
#include "graphics/shader/AmbientShader.h"
#include <iostream>
#include <unordered_map>
 
AmbientShader::AmbientShader(){
    auto sources = FileUtil::map_file("../res/shaders/ambient.shader", {"### VERTEX ###","### FRAGMENT ###"});
    init(sources["### VERTEX ###"], sources["### FRAGMENT ###"]);
} 

AmbientShader::~AmbientShader(){ 
    ;;
} 