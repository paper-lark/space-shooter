#include "Application.h"
#include <glm/gtc/matrix_transform.hpp>

// Process keyboard input
void Application::processKeyboardInput() {
    // Close application
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
        glfwSetWindowShouldClose(this->window, true);
    }

    // Texture mode
    if (glfwGetKey(window, GLFW_KEY_1) == GLFW_PRESS) {
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    }
    if (glfwGetKey(window, GLFW_KEY_2) == GLFW_PRESS) {
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    }

    // Camera movement
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
        camera.move(camera.getDirection() * getDeltaTime());
    }
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
        camera.move(-camera.getDirection() * getDeltaTime());
    }
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
        camera.move(-glm::normalize(glm::cross(camera.getDirection(), camera.getUp())) * getDeltaTime());
    }
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
        camera.move(glm::normalize(glm::cross(camera.getDirection(), camera.getUp())) * getDeltaTime());
    }
}

// Process mouse input
void Application::processMouseInput(GLFWwindow* window, double xpos, double ypos) const {
    // TODO: implement
}


// Update application
void Application::update() {
    // update delta time
    deltaTime = glfwGetTime() - previousMoment;
    previousMoment += deltaTime;

    // process input
    processKeyboardInput();
}


// Get time passed from previous render
float Application::getDeltaTime() const {
    return float(deltaTime);
}