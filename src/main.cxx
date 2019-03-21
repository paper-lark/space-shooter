#define GL_SILENCE_DEPRECATION
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <random>
#include <spdlog/spdlog.h>
#include <iostream>
#include "model/Texture.h"
#include "core/Application.h"
#include "core/Callback.h"
#include "utils/Shader.h"
#include "core/Camera.h"
#include "model/Model.h"
#define WINDOW_HEIGHT 600
#define WINDOW_WIDTH 800
#define WINDOW_TITLE "OpenGL Introduction"


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
        return -1;
    }

    // output graphics adapter information
    SPDLOG_INFO("Vendor: {}", glGetString(GL_VENDOR));
    SPDLOG_INFO("Renderer: {}", glGetString(GL_RENDERER));
    SPDLOG_INFO("Version: {}", glGetString(GL_VERSION));
    SPDLOG_INFO("GLSL: {}", glGetString(GL_SHADING_LANGUAGE_VERSION));

    // enable depth buffer
    glEnable(GL_DEPTH_TEST);

    return 0;
}

// Create vertex buffer with a rectangle
GLuint createVertexBuffer() {
    float vertices[] = {
            // positions          // normals           // texture coords
            -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 0.0f,
             0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 0.0f,
             0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 1.0f,
             0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 1.0f,
            -0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 1.0f,
            -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 0.0f,

            -0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   0.0f, 0.0f,
             0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   1.0f, 0.0f,
             0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   1.0f, 1.0f,
             0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   1.0f, 1.0f,
            -0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   0.0f, 1.0f,
            -0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   0.0f, 0.0f,

            -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 0.0f,
            -0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 1.0f,
            -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
            -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
            -0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 0.0f,
            -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 0.0f,

             0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f,
             0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 1.0f,
             0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
             0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
             0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 0.0f,
             0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f,

            -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 1.0f,
             0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 1.0f,
             0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 0.0f,
             0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 0.0f,
            -0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 0.0f,
            -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 1.0f,

            -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 1.0f,
             0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 1.0f,
             0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 0.0f,
             0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 0.0f,
            -0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 0.0f,
            -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 1.0f
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
GLuint createVertexArrayObject(GLuint vbo) {
    // create vertex array object
    GLuint vertexArrayObject;
    glGenVertexArrays(1, &vertexArrayObject);
    glBindVertexArray(vertexArrayObject);
    createVertexBuffer();
    glBindBuffer(GL_ARRAY_BUFFER, vbo);

    // specify how to interpret vertex data from currently bound VBO
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), nullptr);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
    glEnableVertexAttribArray(2);

    return vertexArrayObject;
}

// Create light source VAO
GLuint createLightArrayObject(GLuint vbo) {
    // create vertex array object
    GLuint lightVAO;
    glGenVertexArrays(1, &lightVAO);
    glBindVertexArray(lightVAO);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);

    // specify how to interpret vertex data from currently bound VBO
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), nullptr);
    glEnableVertexAttribArray(0);

    return lightVAO;
}

struct Light {
    glm::vec3 ambient;
    glm::vec3 diffuse;
    glm::vec3 specular;

    float constant;
    float linear;
    float quadratic;
};

Light getLight() {
    return Light{
        glm::vec3(0.2f, 0.2f, 0.2f),
        glm::vec3(0.5f, 0.5f, 0.5f),
        glm::vec3(1.0f, 1.0f, 1.0f),

        1.0, 0.09, 0.032
    };
}

// Get object source model matrix (local -> world)
glm::mat4 getObjectModelMatrix() {
    glm::mat4 matrix = glm::mat4(1.f);
    return glm::scale(matrix, glm::vec3(0.05f, 0.05f, 0.05f));
}


// Get light source model matrix (local -> world)
glm::mat4 getLightModelMatrix(glm::vec3 lightPosition) {
    glm::mat4 matrix = glm::mat4(1.f);
    matrix = glm::translate(matrix, lightPosition);
    return glm::scale(matrix, glm::vec3(0.1f, 0.1f, 0.1f));
}

// Start game loop that ends when GLFW is signaled to close
void startGameLoop(GLFWwindow* window, Application &app) {
    // create prerequisites
    GLuint cube = createVertexBuffer();
    GLuint light = createLightArrayObject(cube);
    glm::vec3 pointLightPositions[] = {
            glm::vec3( 15.7f, 2.75f, 4.0f),
            glm::vec3( 13.3f, -1.f, -4.0f),
            glm::vec3(-15.0f, 6.0f, -1.0f),
            glm::vec3( 10.25f, 1.75f, -3.0f)
    };

    // create textures and shaders
    Shader objShader = Shader("object/vertex.glsl", "object/fragment.glsl");
    Shader lightShader = Shader("light/vertex.glsl", "light/fragment.glsl");
    Model objectModel{"assets/Spaceship/Arc170.obj"};
    Light lightSpecs = getLight();

    while(!glfwWindowShouldClose(window)) {
        // update application
        app.update();

        // clear screen
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // draw object
        objShader.use();
        objShader.setMatrix("view", app.camera.getViewMatrix());
        objShader.setMatrix("projection", app.camera.getProjectionMatrix());
        objShader.setVec3("viewPos", app.camera.getPos());
        objShader.setMatrix("model", getObjectModelMatrix());

        objShader.setVec3("dirLight.direction", glm::vec3(0.f, 10.f, 5.f));
        objShader.setVec3("dirLight.diffuse", lightSpecs.diffuse);
        objShader.setVec3("dirLight.ambient", lightSpecs.ambient);
        objShader.setVec3("dirLight.specular", lightSpecs.specular);

        objShader.setVec3("flashlight.position", app.camera.getPos());
        objShader.setVec3("flashlight.direction", app.camera.getDirection());
        objShader.setFloat("flashlight.innerCutOff", glm::cos(glm::radians(12.5f)));
        objShader.setFloat("flashlight.outerCutOff", glm::cos(glm::radians(17.5f)));
        objShader.setVec3("flashlight.diffuse", lightSpecs.diffuse);
        objShader.setVec3("flashlight.ambient", lightSpecs.ambient);
        objShader.setVec3("flashlight.specular", lightSpecs.specular);
        objShader.setFloat("flashlight.constant", lightSpecs.constant);
        objShader.setFloat("flashlight.linear", lightSpecs.linear);
        objShader.setFloat("flashlight.quadratic", lightSpecs.quadratic);

        for (int i = 0; i < 4; i++) {
            std::string prefix = "pointLights[" + std::to_string(i) + "]";
            objShader.setVec3(prefix + ".position", pointLightPositions[i]);
            objShader.setVec3(prefix + ".diffuse", lightSpecs.diffuse);
            objShader.setVec3(prefix + ".ambient", lightSpecs.ambient);
            objShader.setVec3(prefix + ".specular", lightSpecs.specular);
            objShader.setFloat(prefix + ".constant", lightSpecs.constant);
            objShader.setFloat(prefix + ".linear", lightSpecs.linear);
            objShader.setFloat(prefix + ".quadratic", lightSpecs.quadratic);
        }

        objectModel.Draw(objShader);


        // draw light sources
        for (int i = 0; i < 4; i++) {
            lightShader.use();
            lightShader.setMatrix("model", getLightModelMatrix(pointLightPositions[i]));
            lightShader.setMatrix("view", app.camera.getViewMatrix());
            lightShader.setMatrix("projection", app.camera.getProjectionMatrix());
            lightShader.setVec3("lightColor", glm::vec3(1.f, 1.f, 1.f));
            glBindVertexArray(light);
            glDrawArrays(GL_TRIANGLES, 0, 36);
        }


        // update color buffers
        glfwSwapBuffers(window); // swap front and back color buffers
        glfwPollEvents(); // process all events
    }

    // release resources
    glDeleteVertexArrays(1, &light);
    glDeleteBuffers(1, &cube);
}

// Entry point
int main(int argc, char **argv) {
    // Initialize logger
    spdlog::set_pattern("%l [%s] %v");
    SPDLOG_INFO("Initializing application...");

    // Create window
    initializeGLFW();
    GLFWwindow* window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, WINDOW_TITLE, nullptr, nullptr);
    if (window == nullptr) {
        SPDLOG_ERROR("Failed to create a window, terminating...");
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED); // attach cursor to the window

    // Initialize application
    Application::initialize(window);

    // Register event callbacks
    glfwSetFramebufferSizeCallback(window, Callback::windowResize);
    glfwSetErrorCallback(Callback::error);
    glfwSetCursorPosCallback(
            window,
            [](GLFWwindow* window, double posX, double posY) {
                Application::getSingleton().processMouseInput(window, posX, posY);
            }
    );

    // Initialize OpenGL
    if (initializeGL() != 0) {
        SPDLOG_ERROR("Failed to initialize OpenGL");
        return -1;
    }

    // Start game loop
    SPDLOG_INFO("Starting game loop...");
    startGameLoop(window, Application::getSingleton());

    // Terminate GLFW
    SPDLOG_INFO("Terminating...");
    glfwTerminate();
    return 0;
}
