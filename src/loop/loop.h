#pragma once

#include <GLFW/glfw3.h>
#include "window/window.h"

namespace Loop{
    // void setUpdateCallback( void (*ucallback)(double) );
    // void setRenderCallback( void (*rcallback)(double) );
    void run(Window::Instance, void (*ucallback)(double), void (*rcallback)(double), int updates_per_second);
}