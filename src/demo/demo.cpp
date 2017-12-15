
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

#include <ctime>
#include <math.h> 
#include "graphics/shader/AmbientShader.h"

#include "graphics/mesh.h"
#include "load/loader.h"


MeshLoader meshloader; 
std::unique_ptr<Mesh> ball;
std::unique_ptr<Mesh> paddle;

mat4 Left_Paddle_Transform;
mat4 Right_Paddle_Transform;
mat4 Ball_Transform;

mat4 Ball_Rotation;
mat4 Ball_Translation;
mat4 Ball_Curve;
mat4 rotate90;

vec3 ball_dir(1,0,0);
float ball_speed = 24;
float ball_rotation = 0.05;

int Left_Paddle_Dir = 0;
int Right_Paddle_Dir = 0;
float paddle_speed = 0.5;

float left_paddle_height = 10;
float right_paddle_height = 10;

bool missed = false;
int paused = 20;

void create_mesh(){
    ball = Load::OBJ(meshloader, "../src/demo/pong/res/mesh/pongball.obj");
    paddle = Load::OBJ(meshloader, "../src/demo/pong/res/mesh/pongpad.obj");
}

void init_transforms(){
    bpm::make_identity(Ball_Transform);
    bpm::make_identity(Left_Paddle_Transform);
    bpm::make_identity(Right_Paddle_Transform);

    bpm::make_identity(Ball_Rotation);
    bpm::make_identity(Ball_Translation);
    bpm::make_identity(Ball_Curve);

    mat4 tmp;
    bpm::make_translation(tmp,vec3(-24,0,0));
    Left_Paddle_Transform = Left_Paddle_Transform * tmp;

    bpm::make_translation(tmp,vec3(24,0,0));
    Right_Paddle_Transform = Right_Paddle_Transform * tmp;

    bpm::make_rotation(tmp,vec3(0,0,1), 3.14159f );
    Right_Paddle_Transform = Right_Paddle_Transform * tmp;

    ball_dir.normalize();
    bpm::make_rotation(Ball_Rotation,vec3(0,0,1), ball_rotation );
    bpm::make_translation(Ball_Translation, ball_speed*ball_dir);
    bpm::make_rotation(rotate90,vec3(0,0,1), 3.14159f/2 );
}


void render_mesh(double t, std::unique_ptr<Mesh>& mesh, mat4& model_transform){

    // BasicShader bshader; 
    AmbientShader ashader;
    ashader.start();  
    ashader.load_colour(1.f, 1.f, 1.f);
    ashader.load_skylight(0.55f, 0.65f, 0.99f);
    ashader.load_groundlight(0.3f, 0.5f, 0.25f);
    ashader.load_upvec(0,0,1);

    vec3 campos( 0, 0, 40);

    mat4 V,P;
    bpm::make_view_lookat(V, campos, vec3(0,0,0), vec3(0,1,0));
    bpm::make_perspective(P, 0.7854f, 1.778f, 0.1f, 100.f);

    ashader.load_transform_model(model_transform); 
    ashader.load_transform_vp(P*V); 

    glBindVertexArray( mesh->vao() );
    glEnableVertexAttribArray(0);//vertices = 0
    glEnableVertexAttribArray(1);//textures = 1
    glEnableVertexAttribArray(2);//normals = 2
 
    glDrawElements(GL_TRIANGLES, mesh->vcount(), GL_UNSIGNED_INT, 0);

    glDisableVertexAttribArray(0); 
    glDisableVertexAttribArray(1); 
    glDisableVertexAttribArray(2); 
    glBindVertexArray(0); 

    ashader.stop();
}
void cleanup(){
    meshloader.unload(std::move(ball));
    meshloader.unload(std::move(paddle));
}

void update_callback(double t, double dt){
    
    if(paused > 0){
        --paused;
    }
    else{
    //physics (n steps)
    int n = 4;
    dt = dt/n;
    for( int i = 0; i < n; ++i)
    {
        //add curve
        vec3 curve_dir = rotate90 * ball_dir;
        float curve_influence = 0.001f*ball_speed*ball_rotation;
        ball_dir += curve_influence*curve_dir;
        ball_dir.normalize();

        //move ball
        bpm::make_translation(Ball_Translation, dt*ball_speed*ball_dir);
        bpm::make_rotation(Ball_Rotation,vec3(0,0,1), ball_rotation );
        Ball_Transform = Ball_Translation * Ball_Transform * Ball_Rotation;

        //check for collisions
        float ballxpos = Ball_Transform.e[3][0];
        float ballypos = Ball_Transform.e[3][1];

        if( ballxpos < -23 || ballxpos > 23 ){
            float paddleypos;
            float paddle_height;
            int rotation_mod;
            float paddle_move;
            if( ballxpos < 0 ){
                paddleypos = Left_Paddle_Transform.e[3][1];
                paddle_height = left_paddle_height;
                paddle_move = Left_Paddle_Dir;
                rotation_mod = -1;
            }
            else{
                paddleypos = Right_Paddle_Transform.e[3][1];
                paddle_height = right_paddle_height;
                paddle_move = Right_Paddle_Dir;
                rotation_mod = 1;
            }

            if( !missed && fabs(ballypos-paddleypos) <= paddle_height/2 ){
                Ball_Transform.e[3][0] = 23*bpm::sign(ballxpos);
                ball_dir = bpm::reflect(ball_dir, vec3(1,0,0));
                ball_rotation += 0.05*paddle_move;
                ball_speed *= 1.05;
            }
            else{
                missed = true;
            }

            //reset
            if( ballxpos < -46 || ballxpos > 46 ){
                ball_dir = bpm::reflect(ball_dir, vec3(1,0,0));
                ball_rotation /= 2;
                ball_speed = 24;
                Ball_Transform.e[3][0] = 0;
                Ball_Transform.e[3][1] = 0;
                missed = false;
                paused = 10;
            }
        }
        if( ballypos < -16 || ballypos > 16 ){
            Ball_Transform.e[3][1] = 16*bpm::sign(ballypos);
            ball_dir = bpm::reflect(ball_dir, vec3(0,1,0));
        }
        
        //increment time
        t += dt;
    }
    }

    mat4 paddle_move;
    bpm::make_translation(paddle_move,vec3(0,paddle_speed*Left_Paddle_Dir,0));
    Left_Paddle_Transform = paddle_move * Left_Paddle_Transform;

    bpm::make_translation(paddle_move,vec3(0,paddle_speed*Right_Paddle_Dir,0));
    Right_Paddle_Transform = paddle_move * Right_Paddle_Transform;

    //post_update
    Events::dispatch_waiting_events();
}

void render_callback(double t, double dt, double alpha){
    glClear(GL_COLOR_BUFFER_BIT);  
    glClear(GL_DEPTH_BUFFER_BIT);
    
    render_mesh(t, paddle, Left_Paddle_Transform);
    render_mesh(t, paddle, Right_Paddle_Transform);
    render_mesh(t, ball, Ball_Transform);
}

static int window_width=1, window_height=1;
void setWindowEvents(Window::Instance window)
{
    glfwSetFramebufferSizeCallback(window.id, [](GLFWwindow* window, int w, int h)->void{
        window_width = w;
        window_height = h;
        glViewport(0, 0, w, h);
    });
    glfwSetKeyCallback(window.id, [](GLFWwindow* window, int key, int scancode, int action, int mods)->void{
        if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS){
            Events::dispatch_now(Events::Window_Event, std::make_unique<Events::Event>(Events::WINDOW_CLOSE)); 
        }
        if (key == GLFW_KEY_UP || key == GLFW_KEY_P) {
            if(action == GLFW_PRESS){
                Right_Paddle_Dir += 1;
            }
            if(action == GLFW_RELEASE){
                Right_Paddle_Dir -= 1;
            }
        }
        if (key == GLFW_KEY_DOWN || key == GLFW_KEY_L) {
            if(action == GLFW_PRESS){
                Right_Paddle_Dir -= 1;
            }
            if(action == GLFW_RELEASE){
                Right_Paddle_Dir += 1;
            }
        }
        if (key == GLFW_KEY_Q || key == GLFW_KEY_W) {
            if(action == GLFW_PRESS){
                Left_Paddle_Dir += 1;
            }
            if(action == GLFW_RELEASE){
                Left_Paddle_Dir -= 1;
            }
        }
        if (key == GLFW_KEY_A || key == GLFW_KEY_S) {
            if(action == GLFW_PRESS){
                Left_Paddle_Dir -= 1;
            }
            if(action == GLFW_RELEASE){
                Left_Paddle_Dir += 1;
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
        auto window = Window::init("Boilerplate", settings);

        Graphics::init(settings); 
        setWindowEvents(window);

        create_mesh();
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
 