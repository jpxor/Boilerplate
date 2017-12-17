
#include "util/fileutil.h"
#include "graphics/shader/WaveShader.h"
#include <iostream>
#include <unordered_map>
 
WaveShader::WaveShader(){
    auto sources = FileUtil::map_file("../res/shaders/wave.shader", {"### VERTEX ###","### FRAGMENT ###"});
    init(sources["### VERTEX ###"], sources["### FRAGMENT ###"]);
} 

WaveShader::~WaveShader(){ 
    ;;
} 