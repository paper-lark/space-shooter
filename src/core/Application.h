#ifndef MAIN_APPLICATION_H
#define MAIN_APPLICATION_H

#define GL_SILENCE_DEPRECATION
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "Callback.h"


class Application {
private:
    GLFWwindow *window = nullptr;

public:
    // Constructor
    Application(GLFWwindow *window): window(window) {}

    // Process input
    void processInput() const;

    // Get delta time from previous render
    double getDeltaTime() const;

    // Get projection matrix (view -> clip)
    glm::mat4 getProjectionMatrix() const;


    // Get View matrix (world -> view)
    glm::mat4 getViewMatrix() const;
};


#endif //MAIN_APPLICATION_H
