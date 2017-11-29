
#include "graphics/glewstatic.h"
#include <string>

static const unsigned int POSITION_ATTR = 0;
static const unsigned int TEXTURE_ATTR = 1;
static const unsigned int NORMAL_ATTR = 2; 

class Shader {
public:
    unsigned int program;
    unsigned int vertex_shader;
    unsigned int fragment_shader;

    //Shader();
    ~Shader();

    virtual void bindAttributes() = 0;
    virtual void getAllUniformLocations() = 0;

    void init(std::string& vs_source, std::string& fs_source);
    void bind_attr(unsigned int index, std::string attr_name);

    inline void start(){
        glUseProgram(program);
    }
    inline void stop(){
        glUseProgram(0);
    }

private:
    int getUniformLocation(std::string uniform_name);
    unsigned int load_shader(std::string& source, unsigned int shader_type);
};
