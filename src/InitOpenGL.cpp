#include "../hdr/InitOpenGL.h"
#include "../hdr/Shader.h"

GLFWwindow* InitOpenGL::Init() {
    GLFWwindow* window = nullptr;
    // Initalizes the Profile of GLFW we are using version 3.3 Core profile
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    // Create a window object
    window = glfwCreateWindow(800, 600, "Learning GL", nullptr, nullptr);

    // Terminate the program if window fails
    if (!window) {
        std::cout << "Failed to create GLFW window\n";
        glfwTerminate();
        return nullptr;
    }

    // Make the window the current context, of the thread?
    glfwMakeContextCurrent(window);

    // Ensure Glad which manages our function ptrs for openGL is initalized (helps makes os specfic things universal)
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cout << "Failed to initalize GLAD\n";
        glfwDestroyWindow(window);
        glfwTerminate();
        return nullptr;
    }

    // Set the size of the viewport (arg 1 and arg 2 sets the location of the lower left of the window)
    glViewport(0,0,800,600);
    return window;
}

void InitOpenGL::framebuffer_size_callback(GLFWwindow* window, int width, int height) {
    glViewport(0, 0, width, height);
}

void InitOpenGL::processInput(GLFWwindow* window) {
    if  (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, true);
    }
}

float InitOpenGL::verticesTriangle3FullPrismo[18] = {
        //Triangle full prismo
        //XYZ : RGB
        0.5, -0.5, 0.0, 1.0, 0.0, 0.0, //bottom Right
        -0.5, -0.5, 0.0, 0.0, 1.0, 0.0, //bot left
        0.0, 0.5, 0.0, 0.0, 0.0, 1.0, //top middle
};

int InitOpenGL::TestOpenGL() {
    GLFWwindow* window = this->Init();
    if (!window) {
        return -1;
    }

    // Register our Viewport resizer
    glfwSetFramebufferSizeCallback(window, InitOpenGL::framebuffer_size_callback);

    Shader s1("shaders/shaderVertex.txt", "shaders/shaderFragment.txt");

    //create Vertex buffer object and the vertex array object
    unsigned int VBO1, VAO1;

    glGenBuffers(1, &VBO1);
    glGenVertexArrays(1, &VAO1);

// Multi color Triangle VBO
    glBindVertexArray(VAO1);
    glBindBuffer(GL_ARRAY_BUFFER, VBO1);
    glBufferData(GL_ARRAY_BUFFER, sizeof(InitOpenGL::verticesTriangle3FullPrismo), InitOpenGL::verticesTriangle3FullPrismo, GL_STATIC_DRAW);
    // Position Atrribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, (6 * sizeof(float)), (void*)0);
    glEnableVertexAttribArray(0);
    // Color Attribute
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, (6 * sizeof(float)), (void*) (3* sizeof(float)));
    glEnableVertexAttribArray(1);
    
    int nrAttributes;
    glGetIntegerv(GL_MAX_VERTEX_ATTRIBS, &nrAttributes);
    std::cout << "Maximum nr of vertex attributes supported: " << nrAttributes << std::endl;

    while (!glfwWindowShouldClose(window)) {
        //wipe any prior colors
        glClearColor(0.2, 0.3, 0.3, 1.0);
        glClear(GL_COLOR_BUFFER_BIT);

        //apply the shader
        s1.use();
        //bind the vertex array
        glBindVertexArray(VAO1);

        //draw triangle
        glDrawArrays(GL_TRIANGLES, 0, 3);


        glBindVertexArray(0);
        //////////////// ^^^ Rendering Commands ^^^ ////////////////
        
        // Swaps Buffers swaps the color buffer (prevents artifcats of seeing a buffer being drawn by creating 2 buffers and only showing the finished buffer as the final product, 1 still buffer to placehold while the other loads the next image)
        glfwSwapBuffers(window);
        // Poll events checks if any events happen like keyboard inputs
        glfwPollEvents(); 

        // Take action on input press
        InitOpenGL::processInput(window);
    }

    glDeleteVertexArrays(1, &VAO1);
    glDeleteBuffers(1, &VBO1);

    glfwTerminate();
    return 0;
}