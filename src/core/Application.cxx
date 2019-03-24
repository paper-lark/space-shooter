#include "Application.h"
#include <glm/gtc/matrix_transform.hpp>
#include <iostream>

Application &Application::instance = *(new Application());

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
void Application::processMouseInput(GLFWwindow *, double posX, double posY) {
  if (player != nullptr) {
    static double lastX = posX, lastY = posY;

    float offsetX = float(posX - lastX) * sensitivity;
    float offsetY = float(posY - lastY) * sensitivity;

    lastX = posX;
    lastY = posY;

    camera.updateRotation(-offsetY, offsetX);
    // TODO: implement rotation
  }
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

// Bind player to the application
void Application::bindPlayer(Player *p) {
  player = p;
}