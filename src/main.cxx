#define GL_SILENCE_DEPRECATION
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <random>
#include <iostream>
#include "utils/LiteMath.h"
#include "utils/Texture.h"
#include "core/Application.h"
#include "core/Callback.h"
#include "utils/Shader.h"
#include "core/Camera.h"
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
    glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
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

    // enable depth buffer
    glEnable(GL_DEPTH_TEST);

    return 0;
}

// Create vertex buffer with a rectangle
GLuint createVertexBuffer() {
    float vertices[] = {
            // positions           // colors           // texture coords
            0.5f,   0.5f, -0.1f,   1.0f, 0.0f, 0.0f,   1.0f, 1.0f,   // top right
            0.5f,  -0.5f, -0.1f,   0.0f, 1.0f, 0.0f,   1.0f, 0.0f,   // bottom right
            -0.5f, -0.5f, -0.1f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f,   // bottom left
            -0.5f,  0.5f, -0.1f,   1.0f, 1.0f, 0.0f,   0.0f, 1.0f,   // top left
            0.5f,   0.5f,  0.1f,   1.0f, 0.0f, 0.0f,   1.0f, 1.0f,   // top right
            0.5f,  -0.5f,  0.1f,   0.0f, 1.0f, 0.0f,   1.0f, 0.0f,   // bottom right
            -0.5f, -0.5f,  0.1f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f,   // bottom left
            -0.5f,  0.5f,  0.1f,   1.0f, 1.0f, 0.0f,   0.0f, 1.0f    // top left
    };

    GLuint vertexBufferObject;
    glGenBuffers(1, &vertexBufferObject); // generate object
    glBindBuffer(GL_ARRAY_BUFFER, vertexBufferObject); // bind buffer to a vertex buffer type
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW); // load vertex data into buffer

    return vertexBufferObject;
}

// Create element buffer object
// Using EBO allows indexed drawing to prevent storing redundant vertices when drawing complex shapes
GLuint createElementBufferObject() {
    unsigned int indices[] = {
            0, 1, 3,
            1, 3, 2,
            1, 2, 6,
            1, 5, 6,
            1, 5, 4,
            1, 0, 4,
            0, 3, 7,
            0, 4, 7,
            2, 6, 7,
            2, 3, 7,
            5, 4, 7,
            5, 6, 7
    };

    GLuint elementBufferObject;
    glGenBuffers(1, &elementBufferObject);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, elementBufferObject);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    return elementBufferObject;
}

// Create vertex array object
// Using VAO allows to configure vertex attribute pointers once and for all
GLuint createVertexArrayObject() {
    // create vertex array object
    GLuint vertexArrayObject;
    glGenVertexArrays(1, &vertexArrayObject);
    glBindVertexArray(vertexArrayObject);
    createVertexBuffer();
    createElementBufferObject();

    // specify how to interpret vertex data from currently bound VBO
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), nullptr);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
    glEnableVertexAttribArray(2);

    return vertexArrayObject;
}

// Get model matrix (local -> world)
glm::mat4 getModelMatrix() {
    return glm::rotate(glm::mat4(1.f), glm::radians(-55.f), glm::vec3(1.f, 0.f, 0.f));
}

// Start game loop that ends when GLFW is signaled to close
void startGameLoop(GLFWwindow* window, Application &app) {
    // create prerequisites
    GLuint vao = createVertexArrayObject();
    Shader shader = Shader("vertex.glsl", "fragment.glsl");
    Texture texture = Texture("assets/wall.jpg");

    while(!glfwWindowShouldClose(window)) {
        // update application
        app.update();

        // clear screen
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // draw
        shader.use();
        shader.setMatrix("model", getModelMatrix());
        shader.setMatrix("view", app.camera.getViewMatrix());
        shader.setMatrix("projection", app.camera.getProjectionMatrix());

        glBindVertexArray(vao);
        texture.bind();
        glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, nullptr);

        // update color buffers
        glfwSwapBuffers(window); // swap front and back color buffers
        glfwPollEvents(); // process all events
    }

    // release resources
    // TODO: release VBO and EBO
    glDeleteVertexArrays(1, &vao);
}

// TODO: read list
// https://learnopengl.com/Getting-started/Shaders
// https://learnopengl.com/In-Practice/Debugging
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
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED); // attach cursor to the window

    // Initialize application
    Camera camera{};
    Application app = Application(window, camera);

    // Register event callbacks
    glfwSetFramebufferSizeCallback(window, Callback::windowResize);
    glfwSetErrorCallback(Callback::error);

    // Initialize OpenGL
    if (initializeGL() != 0) {
        return -1;
    }

    // Start game loop
    startGameLoop(window, app);

    // Terminate GLFW
    glfwTerminate();
    return 0;
}
