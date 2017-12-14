
#include "graphics/glewstatic.h"
#include <GLFW/glfw3.h>
#include <memory>
#include <vector>
#include <stdio.h>
#include <iostream>
#include <thread>
#include <chrono>

#include "window/window.h" 
#include "loop/loop.h" 

using std::vector;
using std::unique_ptr;

namespace Loop{

    using std::chrono::steady_clock;

    void run(Window::Instance window, void (*ucallback)(double,double), void (*rcallback)(double,double,double), int updates_per_second = 60){

        double t = 0;
        double dt = 1.0/updates_per_second;
        double accum = 0;

        auto past = steady_clock::now();

        while( ! window.shouldClose() ){

            auto now = steady_clock::now();
            std::chrono::duration<double> frame_time = (now - past);
            past = now;

            accum += frame_time.count(); 

            /* Update here */
            while( accum >= dt ){
                ucallback(t, dt);
                accum -= dt;
                t += dt;
            }

            const double alpha = accum * updates_per_second;
            //accum is seconds since last update
            //alpha is linear interpolator between present and next states

            /* Render here */
            rcallback(t, accum, alpha);
          
            /* Swap front and back buffers */
            glfwSwapBuffers(window.id);
        
            /* Poll for and process events */
            glfwPollEvents();
        }

    }

}