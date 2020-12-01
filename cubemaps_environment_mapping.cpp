#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <stb_image.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <learnopengl/shader_m.h>
#include "camera.h"

#include <iostream>
#include <vector>

#include "Window.h"
using namespace std;


void error_callback(int error, const char* description)
{
    // Print error.
    std::cerr << description << std::endl;
}

void setup_callbacks(GLFWwindow* window)
{   
    // capture cursor
    //glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    // Set the error callback.
    glfwSetErrorCallback(error_callback);
    // Set the window resize callback.
    glfwSetWindowSizeCallback(window, Window::resizeCallback);

    // Set the key callback.
    glfwSetKeyCallback(window, Window::keyCallback);

    // Set the mouse callback
    //glfwSetMouseButtonCallback(window, Window::mouse_button_callback);
    //glfwSetCursorPosCallback(window, Window::cursor_position_callback);
    //glfwSetScrollCallback(window, Window::scroll_callback);

}

void setup_opengl_settings()
{
    // Enable depth buffering.
    glEnable(GL_DEPTH_TEST);

    // Related to shaders and z value comparisons for the depth buffer.
    glDepthFunc(GL_LEQUAL);

    // Set polygon drawing mode to fill front and back of each polygon.
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

    // Set clear color to black.
    glClearColor(0.0, 0.0, 0.0, 0.0);
}

void print_versions()
{
    // Get info of GPU and supported OpenGL version.
    std::cout << "Renderer: " << glGetString(GL_RENDERER) << std::endl;
    std::cout << "OpenGL version supported: " << glGetString(GL_VERSION)
        << std::endl;

    //If the shading language symbol is defined.
#ifdef GL_SHADING_LANGUAGE_VERSION
    std::cout << "Supported GLSL version is: " <<
        glGetString(GL_SHADING_LANGUAGE_VERSION) << std::endl;
#endif
}

int draw_scene()
{
    GLFWwindow* window = Window::createWindow(800, 600);

    if (!window)
        exit(EXIT_FAILURE);

    // Print OpenGL and GLSL versions.
    print_versions();

    // Setup OpenGL settings.
    setup_opengl_settings();

    // Setup callbacks.
    setup_callbacks(window);

    Window::create_scene();
    
    // render loop
    // -----------
    while (!glfwWindowShouldClose(window))
    {
        Window::idleCallback(window);
        Window::displayCallback(window);
        glfwPollEvents();
    }

    // destroy objects created
    Window::cleanUp();

    // Destroy the window.
    glfwDestroyWindow(window);

    // Terminate GLFW.
    glfwTerminate();
    return 0;
}


int main()
{
    draw_scene();
}






