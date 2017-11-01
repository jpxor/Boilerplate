
#include "graphics/glewstatic.h"
#include <GLFW/glfw3.h>
#include <memory>
#include <vector>
#include <stdio.h>
#include <iostream>
#include <thread>

#include "graphics/mesh.h"
#include "load/loader.h"
#include "window/window.h" 
#include "loop/loop.h" 

using std::vector;
using std::unique_ptr;

void render_mesh(unique_ptr<Mesh>& mesh){
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

namespace Loop{

    void run(Window::Instance window, void (*ucallback)(double), void (*rcallback)(double), int updates_per_second){

        vector<float> positions;
        vector<float> texcoords;
        vector<float> normals;
        vector<int> indices; 
    
        positions.push_back(-0.5f); positions.push_back(-0.5f); positions.push_back( 0.0f);
        positions.push_back( 0.0f); positions.push_back( 0.5f); positions.push_back( 0.0f);
        positions.push_back( 0.5f); positions.push_back(-0.5f); positions.push_back( 0.0f);
    
        texcoords.push_back( 0.0f); texcoords.push_back( 1.0f);
        texcoords.push_back( 0.5f); texcoords.push_back( 0.0f);
        texcoords.push_back( 1.0f); texcoords.push_back( 1.0f);
    
        normals.push_back( 0.0f); normals.push_back( 0.0f); normals.push_back( 1.0f);
        normals.push_back( 0.0f); normals.push_back( 0.0f); normals.push_back( 1.0f);
        normals.push_back( 0.0f); normals.push_back( 0.0f); normals.push_back( 1.0f);
    
        indices.push_back(0);
        indices.push_back(1);
        indices.push_back(2);
        
        MeshLoader meshloader; 
        unique_ptr<Mesh> mesh = meshloader.load(positions, texcoords, normals, indices);

        while( ! window.shouldClose() ){
            /* Render here */
            glClear(GL_COLOR_BUFFER_BIT);  
            render_mesh(mesh);
          
            /* Swap front and back buffers */
            glfwSwapBuffers(window.id);
        
            /* Poll for and process events */
            glfwPollEvents();
        }

        //cleanup
        meshloader.unload(std::move(mesh));
        Window::terminate();
        exit(0);
    }

}