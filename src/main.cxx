#define GL_SILENCE_DEPRECATION
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <random>
#include <spdlog/spdlog.h>
#include <iostream>
#include "representation/Texture.h"
#include "core/Application.h"
#include "core/Callback.h"
#include "representation/Shader.h"
#include "core/Camera.h"
#include "representation/Model.h"
#include "objects/Spaceship.h"
#include "representation/Skybox.h"

#define length(x)  (sizeof(x) / sizeof((x)[0]))
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
    SPDLOG_DEBUG("Device info: {}", glGetString(GL_RENDERER));
    SPDLOG_DEBUG("OpenGL Version: {}, {}", glGetString(GL_VERSION), glGetString(GL_SHADING_LANGUAGE_VERSION));

    // enable depth buffer
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);

    return 0;
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


// Get light source model matrix (local -> world)
glm::mat4 getLightModelMatrix(const glm::vec3 position) {
    glm::mat4 matrix = glm::mat4(1.f);
    matrix = glm::translate(matrix, position);
    matrix = glm::scale(matrix, glm::vec3(30, 30, 30));
    return matrix;
}

// Start game loop that ends when GLFW is signaled to close
void startGameLoop(GLFWwindow* window, Application &app) {
    // create prerequisites
    glm::vec3 lightPositions[] = {
            glm::vec3( 105.7f, 13.75f, 19.0f),
            glm::vec3( 123.3f, -50.f, -55.0f),
            glm::vec3(-35.0f, 76.0f, 83.0f),
            glm::vec3( -58.25f, 33.75f, -143.0f)
    };

    std::vector<Spaceship> spaceships = {
            {100, glm::vec3(10.f, 6.f, -15.f)},
            {100, glm::vec3(23.f, 11.f, 3.f)},
            {1000, glm::vec3(12.f, 5.f, -1.f)},
            {100, glm::vec3(1.f, -12.f, -13.f)},
            {10000, glm::vec3(15.f, 2.f, 22.f)},
            {10000, glm::vec3(-5.f, -12.f, -32.f)},
    };

    std::vector<std::string> textures_faces = {
            "assets/Skybox/lightblue/right.png",
            "assets/Skybox/lightblue/left.png",
            "assets/Skybox/lightblue/top.png",
            "assets/Skybox/lightblue/bot.png",
            "assets/Skybox/lightblue/front.png",
            "assets/Skybox/lightblue/back.png"
    };
    Skybox skybox{textures_faces, Shader("skybox/vertex.glsl", "skybox/fragment.glsl")};
    Model starModel{"assets/Star/Mercury 1K.obj"}; // TODO: move to object

    // create textures and shaders
    Shader objShader = Shader("object/vertex.glsl", "object/fragment.glsl");
    Shader lightShader = Shader("light/vertex.glsl", "light/fragment.glsl");
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

        for (int i = 0; i < length(lightPositions); i++) {
            std::string prefix = "pointLights[" + std::to_string(i) + "]";
            objShader.setVec3(prefix + ".position", lightPositions[i]);
            objShader.setVec3(prefix + ".diffuse", lightSpecs.diffuse);
            objShader.setVec3(prefix + ".ambient", lightSpecs.ambient);
            objShader.setVec3(prefix + ".specular", lightSpecs.specular);
            objShader.setFloat(prefix + ".constant", lightSpecs.constant);
            objShader.setFloat(prefix + ".linear", lightSpecs.linear);
            objShader.setFloat(prefix + ".quadratic", lightSpecs.quadratic);
        }

        for (Spaceship &ship: spaceships) {
            ship.Draw(objShader);
            ship.Update(app.getDeltaTime());
        }


        // draw light sources
        lightShader.use();
        lightShader.setMatrix("view", app.camera.getViewMatrix());
        lightShader.setMatrix("projection", app.camera.getProjectionMatrix());
        for (int i = 0; i < length(lightPositions); i++) {
            lightShader.setMatrix("model", getLightModelMatrix(lightPositions[i]));
            starModel.Draw(lightShader);
        }

        // draw skybox
        skybox.Draw(app.camera);

        // update color buffers
        glfwSwapBuffers(window); // swap front and back color buffers
        glfwPollEvents(); // process all events

        // remove all dead objects
        spaceships.erase(
                std::remove_if(spaceships.begin(), spaceships.end(), [](const Spaceship &s){ return !s.IsAlive(); }),
                spaceships.end()
        );
    }
}

// Entry point
int main(int argc, char **argv) {
    // Initialize logger
    spdlog::set_pattern("%T – %l [%s] %v");
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
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

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

    // Load all models
    SPDLOG_INFO("Loading models...");
    Spaceship::Init();

    // Start game loop
    SPDLOG_INFO("Starting game loop...");
    startGameLoop(window, Application::getSingleton());

    // Terminate GLFW
    SPDLOG_INFO("Terminating...");
    Spaceship::Release();
    glfwTerminate();
    return 0;
}
