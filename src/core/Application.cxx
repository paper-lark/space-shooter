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

  // Camera mode
  if (glfwGetKey(window, GLFW_KEY_3) == GLFW_PRESS) {
    cameraPosition = CameraPosition::ThirdPerson;
  }
  if (glfwGetKey(window, GLFW_KEY_4) == GLFW_PRESS) {
    cameraPosition = CameraPosition::FirstPerson;
  }

  // Player movement
  if (player != nullptr) {
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
      player->updateSpeed(0.2f * getDeltaTime());
    }
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
      player->updateSpeed(-0.175f * getDeltaTime());
    }
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

    glm::quat qPitch = glm::angleAxis(glm::radians(-offsetY), glm::vec3(1, 0, 0));
    glm::quat qYaw = glm::angleAxis(glm::radians(offsetX), glm::vec3(0, 1, 0));

    player->rotate(qPitch * qYaw);
  }
}

// Update application
void Application::update() {
  // update delta time
  deltaTime = glfwGetTime() - previousMoment;
  previousMoment += deltaTime;

  // process input
  processKeyboardInput();

  // update camera position and orientation
  if (player != nullptr) {
    auto orientation = player->getOrientation();
    glm::vec3 cameraOffset = cameraPosition == CameraPosition::FirstPerson ?
        + 5.f * QuatHelpers::getForward(orientation) :
        - 15.f * QuatHelpers::getForward(orientation) + QuatHelpers::getUp(orientation) * 3.f;
    camera.updatePosition(player->getPosition() + cameraOffset);
    camera.setOrientation(player->getOrientation());
  }
}

// Get time passed from previous render
float Application::getDeltaTime() const {
  return float(deltaTime);
}

// Bind player to the application
void Application::bindPlayer(Player *p) {
  player = p;
}

unsigned Application::getScore() const {
  return score;
}

glm::ivec2 Application::getWindowSize() const {
  glm::ivec2 size;
  glfwGetWindowSize(window, &size.x, &size.y);
  return size;
}