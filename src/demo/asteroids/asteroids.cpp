
#include "graphics/glewstatic.h"
#include <GLFW/glfw3.h>
#include <memory>
#include <vector>
#include <iostream>
#include <thread>
#include <string>
#include <ctime>
#include <math.h>
#include <random>

#include "console/console.h"
#include "window/window.h"
#include "settings/config.h"
#include "loop/loop.h"
#include "events/eventdispatch.h"
#include "events/eventenums.h"

#include "graphics/ginit.h"
#include "graphics/mesh.h"
#include "graphics/material.h"

#include "load/loader.h"


MeshLoader meshloader; 
mat4 Projection, View, ship_transform;
vec3 campos(0, 0, 20);
vec3 target_campos( 0, 0, 80);
vec2 target_position(0,0);

vec3 ship_pos(0, 0, 0);
vec3 ship_vel(0, 0, 0);
float ship_acc = 100;
float ship_rot(0); 


mat4 ship_orb_rotation;
float ship_orb_angle = 0;
float ship_orb_rot_speed = 3.14f;

MaterialShader mat_shader;

std::vector<std::shared_ptr<Model>> asteroid_models;
int spawn_index = 0;
int asteroid_cap = 19;
std::vector<int> asteroid_model_indices;
std::vector<bpm::vec3> asteroid_positions;
std::vector<bpm::vec3> asteroid_directions;
std::vector<float> asteroid_rotations;
std::vector<bpm::vec3> asteroid_rotation_axis;

std::vector<std::shared_ptr<Model>> player_ship;
int ship_forwards = 0;
int ship_rightwards = 0;

std::vector<bpm::vec3> light_positions = {
    bpm::vec3( 20, 0, 20),
    bpm::vec3(-20, 0, 20),
    bpm::vec3( 0, 20, 20),
    bpm::vec3( 0, 0, -20)
};

std::vector<bpm::vec3> light_colors = {
    bpm::vec3(2,0,0),
    bpm::vec3(0,2,0),
    bpm::vec3(0,0,2),
    bpm::vec3(1,1,1)
};

std::vector<bpm::vec3> light_attenuation = {
    bpm::vec3(1, 0.1f, 0.01f),
    bpm::vec3(1, 0.1f, 0.01f),
    bpm::vec3(1, 0.1f, 0.01f),
    bpm::vec3(1, 0.1f, 0.01f)
};

void load_ship_model(){
    player_ship = Load::obj_model(meshloader, "../res/demo/asteroids/minership.obj");
    std::cout << "ship loaded" << std::endl;

    mat_shader.init();
    std::cout << "shader initialized" << std::endl;
}

void load_asteroid_models(){
    asteroid_models = Load::obj_model(meshloader, "../res/demo/asteroids/asteroids.obj");

    std::random_device rd;
    std::default_random_engine e1(rd());
    std::uniform_real_distribution<> urand(-1,1);
    
    while(asteroid_model_indices.size() < asteroid_cap) {
        asteroid_model_indices.push_back( spawn_index );
        spawn_index = (spawn_index+1) % asteroid_models.size();

        bpm::vec3 rot = bpm::vec3(urand(e1), urand(e1), urand(e1)).normalize();
        asteroid_rotation_axis.push_back(rot);
        asteroid_rotations.push_back(urand(e1));

        bpm::vec3 start_pos = bpm::vec3(70*urand(e1), 32*urand(e1), 0);
        asteroid_positions.push_back(start_pos);

        bpm::vec3 target_pos = bpm::vec3(58*urand(e1), 35+32*urand(e1), 0);
        bpm::vec3 dir = (target_pos - start_pos).normalize();
        asteroid_directions.push_back(dir);
    }
    std::vector<bpm::vec3> asteroid_directions;
}

void init_transforms(){
    bpm::make_perspective(Projection, 0.7854f, 1.778f, 0.1f, 100.f);
    bpm::make_view_lookat(View, campos, vec3(0,0,0), vec3(0,1,0));
}

void cleanup(){
    
}

void update_callback(double t, double dt) 
{
    for(int i = 0; i < asteroid_model_indices.size(); ++i) {
        asteroid_positions[i] += 0.05f * asteroid_directions[i];
        asteroid_rotations[i] += 0.01f;
    }

    ship_orb_angle += ship_orb_rot_speed*dt;
    bpm::make_rotation(ship_orb_rotation, bpm::vec3(0,1,0), ship_orb_angle);

    ship_vel *= 0.95f;
    if(ship_rightwards != 0 || ship_forwards != 0 ){
        bpm::vec3 dv = dt * ship_acc * vec3(ship_rightwards, ship_forwards, 0).normalize();
        ship_vel = ship_vel + dv;
    }
    ship_pos += dt*ship_vel;

    static bpm::vec3 light3_offset = bpm::vec3(0,0,3);
    light_positions[3] = ship_pos + light3_offset;

    float target_ship_rot = atan2(target_position.y - (ship_pos.y/32), target_position.x - (ship_pos.x/58) ) - 1.57;
    float drot = target_ship_rot - ship_rot;
    while(drot >  3.14) drot -= 6.28;
    while(drot < -3.14) drot += 6.28;
    ship_rot += drot/2/2;

    bpm::mat4 translation, rotation;
    bpm::make_translation(translation,ship_pos);
    bpm::make_rotation(rotation, bpm::vec3(0,0,1), ship_rot);
    ship_transform = translation * rotation;

    campos += dt*(target_campos-campos);
    bpm::make_view(View, campos, vec3(0,0,-1), vec3(0,1,0));

    //post_update
    Events::dispatch_waiting_events();
}


void render_player_ship(double t, double dt, double alpha){

    mat_shader.start();

    // load common + environment settings
    mat_shader.load("view", View);
    mat_shader.load("projection", Projection);
    mat_shader.load("fog_density", 0.2f);
    mat_shader.load("fog_visibility", 200.0f);
    mat_shader.load("fog_color", bpm::vec3(0.8f, 0.8f, 0.8f));
    mat_shader.load("ambient_light", bpm::vec3(0.12f, 0.12f, 0.12f) );

    int i = 0;
    for( auto section : player_ship ) {
        auto material = section->material;
        auto mesh = section->mesh;

        // load mesh + position dependant settings
        if(i++ == 4){
            mat_shader.load("model", ship_transform*ship_orb_rotation);
        }
        else{
            mat_shader.load("model", ship_transform);
        }
        mat_shader.load("light_positions", light_positions);
        mat_shader.load("light_colors", light_colors);
        mat_shader.load("light_attenuation", light_attenuation);

        // load material settings
        material->apply(mat_shader);

        // render
        glBindVertexArray( mesh->vao() );
        glEnableVertexAttribArray(0);//vertices = 0
        glEnableVertexAttribArray(1);//textures = 1
        glEnableVertexAttribArray(2);//normals = 2
 
        glDrawElements(GL_TRIANGLES, mesh->vcount(), GL_UNSIGNED_INT, 0);

        glDisableVertexAttribArray(0); 
        glDisableVertexAttribArray(1); 
        glDisableVertexAttribArray(2); 
        glBindVertexArray(0); 
    }

    mat_shader.stop();
}

void render_asteroids(double t, double dt, double alpha){

    mat_shader.start();

    // load common + environment settings
    mat_shader.load("view", View);
    mat_shader.load("projection", Projection);
    mat_shader.load("fog_density", 0.2f);
    mat_shader.load("fog_visibility", 200.0f);
    mat_shader.load("fog_color", bpm::vec3(0.8f, 0.8f, 0.8f));
    mat_shader.load("ambient_light", bpm::vec3(0.12f, 0.12f, 0.12f) );
    
    mat_shader.load("light_positions", light_positions);
    mat_shader.load("light_colors", light_colors);
    mat_shader.load("light_attenuation", light_attenuation);

    for(int i = 0; i < asteroid_model_indices.size(); ++i) {

        bpm::mat4 translation, rotation;
        bpm::make_translation(translation, asteroid_positions[i]); 
        bpm::make_rotation(rotation, asteroid_rotation_axis[i], asteroid_rotations[i]); 
        mat_shader.load("model", translation*rotation);

        auto model = asteroid_models[asteroid_model_indices[i]]; 
        auto material = model->material;
        auto mesh = model->mesh;

        // load material settings
        material->apply(mat_shader);

        // render
        glBindVertexArray( mesh->vao() );
        glEnableVertexAttribArray(0);//vertices = 0
        glEnableVertexAttribArray(1);//textures = 1
        glEnableVertexAttribArray(2);//normals = 2
 
        glDrawElements(GL_TRIANGLES, mesh->vcount(), GL_UNSIGNED_INT, 0);

        glDisableVertexAttribArray(0); 
        glDisableVertexAttribArray(1); 
        glDisableVertexAttribArray(2); 
        glBindVertexArray(0); 
    }
    mat_shader.stop();
}

void render_callback(double t, double dt, double alpha){
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);  
    render_player_ship(t,dt,alpha);
    render_asteroids(t,dt,alpha);
}

static int window_width=1, window_height=1;
void setWindowEvents(Window::Instance window)
{
    glfwGetWindowSize(window.id, &window_width, &window_height);
    glfwSetFramebufferSizeCallback(window.id, [](GLFWwindow* window, int w, int h)->void{
        window_width = w;
        window_height = h;
        glViewport(0, 0, w, h);
    });
    glfwSetCursorPosCallback(window.id, [](GLFWwindow* window, double xpos, double ypos)->void{
        target_position.x = 2*(xpos/window_width - 0.5f);
        target_position.y = -2*(ypos/window_height - 0.5f);
    });
    glfwSetKeyCallback(window.id, [](GLFWwindow* window, int key, int scancode, int action, int mods)->void{
        if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS){
            Events::dispatch_now(Events::Window_Event, std::make_unique<Events::Event>(Events::WINDOW_CLOSE)); 
        }
        if (key == GLFW_KEY_UP || key == GLFW_KEY_W) {
            if(action == GLFW_PRESS){
                ship_forwards += 1;
            }
            if(action == GLFW_RELEASE){
                ship_forwards -= 1;
            }
        }
        if (key == GLFW_KEY_DOWN || key == GLFW_KEY_S) {
            if(action == GLFW_PRESS){
                ship_forwards -= 1;
            }
            if(action == GLFW_RELEASE){
                ship_forwards += 1;
            }
        }
        if (key == GLFW_KEY_RIGHT || key == GLFW_KEY_D) {
            if(action == GLFW_PRESS){
                ship_rightwards += 1;
            }
            if(action == GLFW_RELEASE){
                ship_rightwards -= 1;
            }
        }
        if (key == GLFW_KEY_LEFT || key == GLFW_KEY_A) {
            if(action == GLFW_PRESS){
                ship_rightwards -= 1;
            }
            if(action == GLFW_RELEASE){
                ship_rightwards += 1;
            }
        }
    });
    Events::subscribe(Events::Window_Event, [window](auto& event)->void{
        if(event->id == Events::WINDOW_CLOSE){
            window.close();
        }
    });
}

int main(){
    std::thread consoleThread(Console::start);
    try{
        auto settings = Settings::load(); 
        auto window = Window::init("Asteroids", settings);

        Graphics::init(settings); 
        setWindowEvents(window);

        load_ship_model();
        load_asteroid_models();
        init_transforms();
        
        Loop::run(window, update_callback, render_callback, 40);
    }
    catch(const std::exception& e){
        std::cout << "Exception Caught: " << e.what() << std::endl;
    }
    
    Window::terminate();
    cleanup();
    exit(0);
} 
 