
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

void update_callback(double dt){
    //pre_update(dt);
    //update(dt);
    //post_update(dt);
    Events::dispatch_waiting_events();
}

void render_callback(double dt){
    //pre_render(dt);
    //render(dt);
    //post_render(dt);
}

#include "util/fileutil.h"

int main(){

    //test file utils
    { 
        std::list<std::string> shader_keys = {"### VERTEX ###", "### FRAGMENT ###", };
        std::unordered_map<std::string,std::string> shaders;
        shaders = FileUtil::map_file("../res/shaders/basic.shader", shader_keys);
        for (auto it : shaders) {
            std::cout << it.first << ":\n" << it.second << "\n\n";
        }
    }
    
    std::thread consoleThread(Console::start);
    try{
        auto settings = Settings::load(); 
        auto window = Window::init("Boilerplate", settings);

        Graphics::init(settings); 
        setWindowEvents(window);
        Loop::run(window, update_callback, render_callback, 40);
    }
    catch(const std::exception& e){
        std::cout << "Exception Caught: " << e.what() << std::endl;
    }
    consoleThread.join();
    Window::terminate();
    exit(0);
}
