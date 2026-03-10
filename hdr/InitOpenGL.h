#ifndef INITOPENGL_H
#define INITOPENGL_H

#include <glad/glad.h>
#include <glfw3.h>
#include <iostream>

class InitOpenGL {
    public:
    InitOpenGL() = default;

    // Initalizes a window and the viewport
    GLFWwindow* Init();

    //allows for the viewport to resize correctly
    static void framebuffer_size_callback(GLFWwindow* window, int width, int height);

    //allows for input to have custom action
    static void processInput(GLFWwindow* window);

    //Prismo triangle for testing purpses
    static float verticesTriangle3FullPrismo[18];

    //display a simple rgb triangle window
    int TestOpenGL();

};

#endif