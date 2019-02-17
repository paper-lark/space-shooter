#define GL_SILENCE_DEPRECATION
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <random>
#include <fstream>
#include <iostream>
#include "LiteMath.h"
#include "Application.h"
#include "Callback.h"
//#include "common.h"
#define WINDOW_HEIGHT 600
#define WINDOW_WIDTH 800
#define WINDOW_TITLE "OpenGL Introduction"

using namespace LiteMath;

// Initialize GLFW
void initializeGLFW() {
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
}

// Initialize OpenGL
int initializeGL() {
    // load OpenGL with GLAD
    if (!gladLoadGLLoader((GLADloadproc) glfwGetProcAddress)) {
        std::cout << "Failed to initialize OpenGL context" << std::endl;
        return -1;
    }

    // output graphics adapter information
    std::cout << "Vendor: " << glGetString(GL_VENDOR) << std::endl;
    std::cout << "Renderer: " << glGetString(GL_RENDERER) << std::endl;
    std::cout << "Version: " << glGetString(GL_VERSION) << std::endl;
    std::cout << "GLSL: " << glGetString(GL_SHADING_LANGUAGE_VERSION) << std::endl;

    return 0;
}

// Load shader specified by its source file and type
GLuint loadShader(GLenum type, const std::string &filename) {
    // read shader from file
    std::ifstream fs(filename);
    if (!fs.is_open()) {
        std::cerr << "Failed to load shader file: " << filename << std::endl;
        throw std::runtime_error("Shader missing");
    }
    std::string shaderText((std::istreambuf_iterator<char>(fs)), std::istreambuf_iterator<char>());

    // compile shader
    GLuint shaderObject = glCreateShader(type); // create shader object
    const GLchar *source = shaderText.c_str();
    glShaderSource(shaderObject, 1, &source, nullptr);
    glCompileShader(shaderObject);

    // check for compilation errors
    GLint compilationStatus;
    glGetShaderiv(shaderObject, GL_COMPILE_STATUS, &compilationStatus);

    if (compilationStatus != GL_TRUE) {
        GLchar infoLog[512];
        glGetShaderInfoLog(shaderObject, 512, nullptr, infoLog);
        std::cerr << "Shader compilation failed: " << std::endl << infoLog << std::endl;
        throw std::runtime_error("Shader compilation failed");
    }

    return shaderObject;
}

// Create vertex buffer with a triangle
GLuint createVertexBuffer() {
    float vertices[] = {
            -0.5f, -0.5f, 0,
            0.5f, -0.5f, 0,
            0, 0.5f, 0
    };

    GLuint vertexBufferObject;
    glGenBuffers(1, &vertexBufferObject); // generate object
    glBindBuffer(GL_ARRAY_BUFFER, vertexBufferObject); // bind buffer to a vertex buffer type
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW); // load vertex data into buffer

    return vertexBufferObject;
}

// Create vertex array object
GLuint createVertexArrayObject() {
    // create vertex array object
    GLuint vertexArrayObject;
    glGenVertexArrays(1, &vertexArrayObject);
    glBindVertexArray(vertexArrayObject);
    createVertexBuffer();

    // specify how to interpret vertex data from currently bound VBO
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*) 0);
    glEnableVertexAttribArray(0);

    return vertexArrayObject;
}

// Create shader program
GLuint createShaderProgram() {
    GLuint vertexShader = loadShader(GL_VERTEX_SHADER, "vertex.glsl");
    GLuint fragmentShader = loadShader(GL_FRAGMENT_SHADER, "fragment.glsl");

    // create shader program
    GLuint shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    // check for errors
    GLint success;
    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    if (success != GL_TRUE) {
        GLchar infoLog[512];
        glGetProgramInfoLog(shaderProgram, 512, nullptr, infoLog);
        std::cerr << "Shader linking failed: " << std::endl << infoLog << std::endl;
        throw std::runtime_error("Shader linking failed");
    }

    return shaderProgram;
}

// Start game loop that ends when GLFW is signaled to close
void startGameLoop(GLFWwindow* window) {

    // load shader program and vertex array object
    GLuint shaderProgram = createShaderProgram();
    GLuint vao = createVertexArrayObject();


    // create application
    Application app = Application(window);

    while(!glfwWindowShouldClose(window)) {
        // process input
        app.processInput();

        // clear screen
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        // draw
        glUseProgram(shaderProgram);
        glBindVertexArray(vao);
        glDrawArrays(GL_TRIANGLES, 0, 3);

        // update color buffers
        glfwSwapBuffers(window); // swap front and back color buffers
        glfwPollEvents(); // process all events
    }
}

int main(int argc, char **argv) {
    // Create window
    initializeGLFW();
    GLFWwindow* window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, WINDOW_TITLE, nullptr, nullptr);
    if (window == nullptr) {
        std::cerr << "Failed to create a window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);

    // Register event callbacks
    glfwSetFramebufferSizeCallback(window, Callback::windowResize);
    glfwSetErrorCallback(Callback::error);

    // Initialize OpenGL
    if (initializeGL() != 0) {
        return -1;
    }

    // Start game loop
    startGameLoop(window);

    // Terminate GLFW
    glfwTerminate();
    return 0;
}
