#ifndef MAIN_APPLICATION_H
#define MAIN_APPLICATION_H

#define GL_SILENCE_DEPRECATION
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <GLFW/glfw3.h>
#include "Camera.h"


class Application {
private:
    GLFWwindow *window;
    double previousMoment = 0.0;
    double deltaTime = 0.0;
    const float sensitivity = 0.05f; // mouse sensitivity
    static Application &instance;

public:
    // Application camera object
    Camera camera;

    // Constructor
    Application(GLFWwindow *window = nullptr): window(window) {}

    // Process keyboard input
    void processKeyboardInput();

    // Process mouse input
    void processMouseInput(GLFWwindow* window, double posX, double posY);

    // Update application state
    void update();

    // Get delta time from previous render
    float getDeltaTime() const;

    // Initialize application singleton
    static void initialize(GLFWwindow *window) {
        Application::instance.window = window;
    }

    // Get application singleton
    static Application &getSingleton() {
        return Application::instance;
    }
};

#endif //MAIN_APPLICATION_H
