
#include "graphics/glewstatic.h"
#include <GLFW/glfw3.h>
#include <memory>
#include <vector>
#include <iostream>
#include <thread>
#include <string>

#include "console/console.h"
#include "window/window.h"
#include "settings/config.h"
#include "loop/loop.h"
#include "events/eventdispatch.h"
#include "events/eventenums.h"
#include "graphics/ginit.h"



void setWindowEvents(Window::Instance window)
{
    glfwSetKeyCallback(window.id, [](GLFWwindow* window, int key, int scancode, int action, int mods)->void{
        if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS){
            Events::dispatch_now(Events::Window_Event, std::make_unique<Events::Event>(Events::WINDOW_CLOSE)); 
        }
    });
    Events::subscribe(Events::Window_Event, [window](auto& event)->void{
        if(event->id == Events::WINDOW_CLOSE){
            window.close();
        }
    });
}

#include "graphics/mesh.h"
#include "load/loader.h"

MeshLoader meshloader; 
std::unique_ptr<Mesh> mesh;

void create_mesh(){
    mesh = Load::OBJ(meshloader, "../res/smooth-suzanne.obj");
}

#include <ctime>
#include <math.h> 
#include "graphics/shader/BasicShader.h"
void render_mesh(double t, std::unique_ptr<Mesh>& mesh){

    BasicShader bshader; 
    bshader.start(); 
    bshader.load_color(0.8f, 0.25f, 0.8f);

    float rad = 4;
    vec3 pos( rad*sin(t) , rad*cos(t) , 1);

    mat4 M,V,P;
    bpm::make_identity(M);
    bpm::make_view_lookat(V, pos, vec3(0,0,0), vec3(0,0,1));
    bpm::make_perspective(P, 0.7854f, 1, 0.1f, 100.f);
    bshader.load_transform(P*V*M); 

    glBindVertexArray( mesh->vao() );
    glEnableVertexAttribArray(0);//vertices = 0
    glEnableVertexAttribArray(1);//textures = 1
    glEnableVertexAttribArray(2);//normals = 2
 
    glDrawElements(GL_TRIANGLES, mesh->vcount(), GL_UNSIGNED_INT, 0);

    glDisableVertexAttribArray(0); 
    glDisableVertexAttribArray(1); 
    glDisableVertexAttribArray(2); 
    glBindVertexArray(0); 

    bshader.stop();
}
void cleanup(){
    meshloader.unload(std::move(mesh));
}

void update_callback(double t, double dt){
    //pre_update(dt);
    Events::dispatch_waiting_events();
    //update(dt);
    Events::dispatch_waiting_events();
    //post_update(dt);
    Events::dispatch_waiting_events();
}

void render_callback(double t, double dt, double alpha){
    //pre_render(dt);
    //render(dt);
    //post_render(dt);

    glClear(GL_COLOR_BUFFER_BIT);  
    render_mesh(t, mesh);
}

// #include "test.h"
int main(){

    //bptest::run();

    std::thread consoleThread(Console::start);
    try{
        auto settings = Settings::load(); 
        auto window = Window::init("Boilerplate", settings);

        Graphics::init(settings); 
        setWindowEvents(window);

        create_mesh();
        Loop::run(window, update_callback, render_callback, 40);
    }
    catch(const std::exception& e){
        std::cout << "Exception Caught: " << e.what() << std::endl;
    }
    
    Window::terminate();
    cleanup();
    exit(0);
} 
 