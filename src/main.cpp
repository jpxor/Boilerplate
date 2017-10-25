
#include "glewstatic.h"
#include <GLFW/glfw3.h>
#include <memory>
#include <vector>
#include <stdio.h>
#include <mesh.h>
#include <loader.h>
 
using std::vector;
using std::unique_ptr;

void render(unique_ptr<Mesh>& mesh){
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

int main(void)
{
    GLFWwindow* window;

    /* Initialize the library */
    if (!glfwInit())
        return -1;

    /* Create a windowed mode window and its OpenGL context */
    window = glfwCreateWindow(640, 480, "Boilerplate", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        return -1;
    }

    /* Make the window's context current */
    glfwMakeContextCurrent(window);

    GLenum err = glewInit();
    if (GLEW_OK != err)
    {
      /* Problem: glewInit failed, something is seriously wrong. */
      fprintf(stderr, "Error: %s\n", glewGetErrorString(err));
      //TODO 
    }
    fprintf(stdout, "Status: Using GLEW %s\n", glewGetString(GLEW_VERSION));

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
    auto mesh = meshloader.load(positions, texcoords, normals, indices);

    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window))
    {
        /* Render here */
        glClear(GL_COLOR_BUFFER_BIT);

        render(mesh);

        /* Swap front and back buffers */
        glfwSwapBuffers(window);

        /* Poll for and process events */
        glfwPollEvents();
    }

    //cleanup
    meshloader.unload(std::move(mesh));

    glfwTerminate();
    return 0;
}
