
#include "graphics/shader/shader.h"
#include <unordered_map>

class BasicShader : Shader {
    public:
    BasicShader();
    ~BasicShader();

    // load_color(float r, float g, float b){
    //     load_vec3( var_locations["location_color"], r,g,b ); 
    // }

    // load_view(){
    //     load_mat4( var_locations["location_view"] );
    // }

    // load_projection(){
    //     load_mat4( var_locations["location_projection"] );
    // }

    // load_transform(){
    //     load_mat4( var_locations["location_transform"] );
    // }
    
    private: 
    std::unordered_map<std::string, int> var_locations;

    void bindAttributes(){
        bind_attr(POSITION_ATTR, "position");
        bind_attr(TEXTURE_ATTR, "texcoord");
        bind_attr(NORMAL_ATTR, "normal");
    }

    void getAllUniformLocations(){
        
    }
};