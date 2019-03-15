#ifndef MAIN_APPLICATION_H
#define MAIN_APPLICATION_H

#define GL_SILENCE_DEPRECATION
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <GLFW/glfw3.h>
#include "Camera.h"


class Application {
private:
    GLFWwindow *window = nullptr;
    double previousMoment = 0.0;
    double deltaTime = 0.0;
public:
    // Application camera object
    Camera &camera;

    // Constructor
    Application(GLFWwindow *window, Camera &camera): window(window), camera(camera) {}

    // Process keyboard input
    void processKeyboardInput();

    // Process mouse input
    void processMouseInput(GLFWwindow* window, double xpos, double ypos) const;

    // Update application state
    void update();

    // Get delta time from previous render
    float getDeltaTime() const;

};


#endif //MAIN_APPLICATION_H
