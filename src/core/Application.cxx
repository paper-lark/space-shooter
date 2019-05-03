#include "Application.h"
#include "../objects/Torpedo.h"
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

  // Player actions
  if (scene != nullptr) {
    Player *player = scene->getPlayer();
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
      player->updateSpeed(0.5f * getDeltaTime());
    }
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
      player->updateSpeed(-0.275f * getDeltaTime());
    }
    if(glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS) {
      double now = glfwGetTime();
      if (now >= lastFireTime + fireCooldown) {
        scene->addObject(new Torpedo{
            1000,
            player->getPosition() + 3.f * QuatHelpers::getForward(player->getOrientation()),
            player->getOrientation()
        });
        lastFireTime = now;
      }
    }
  }
}

// Process mouse input
void Application::processMouseInput(GLFWwindow *, double posX, double posY) {
  if (scene != nullptr) {
    Player *player = scene->getPlayer();
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
  if (scene != nullptr) {
    Player *player = scene->getPlayer();
    auto orientation = player->getOrientation();
    glm::vec3 cameraOffset = cameraPosition == CameraPosition::FirstPerson ?
        + 1.f * QuatHelpers::getForward(orientation) + QuatHelpers::getUp(orientation) * 2.5f :
        - 10.f * QuatHelpers::getForward(orientation) + QuatHelpers::getUp(orientation) * 2.5f;
    camera.updatePosition(player->getPosition() + cameraOffset);
    camera.setOrientation(player->getOrientation());
  }
}

// Get time passed from previous render
float Application::getDeltaTime() const {
  return float(deltaTime);
}

// Bind player to the application
void Application::bindScene(Scene *sc) {
  scene = sc;
}

unsigned Application::getScore() const {
  return score;
}

glm::ivec2 Application::getWindowSize() const {
  glm::ivec2 size;
  glfwGetWindowSize(window, &size.x, &size.y);
  return size;
}