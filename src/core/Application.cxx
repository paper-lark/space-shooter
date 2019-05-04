#include "Application.h"
#include "../objects/Torpedo.h"
#include <glm/gtc/matrix_transform.hpp>
#include <iostream>

#define FPS_Z_OFFSET 1.f
#define FPS_Y_OFFSET 0.f
#define TPS_Z_OFFSET -15.f
#define TPS_Y_OFFSET 2.5f

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

    // Movement
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
      player->moveInPlane(Direction::Left);
    }
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
      player->moveInPlane(Direction::Right);
    }
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
      player->moveInPlane(Direction::Up);
    }
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
      player->moveInPlane(Direction::Down);
    }

    // Fire
    if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS) {
      double now = glfwGetTime();
      if (now >= lastFireTime + PLAYER_FIRE_COOLDOWN) {
        scene->createTorpedo(player->getPosition(), player->getTargetDirection());
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
    float offsetX = float(posX - lastX) * MOUSE_SENSITIVITY;
    float offsetY = float(posY - lastY) * MOUSE_SENSITIVITY;
    lastX = posX;
    lastY = posY;
    player->rotateTarget(-offsetY, offsetX);
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
  this->updateCameraPosition();
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

void Application::updateScore(unsigned delta) {
  score += delta;
}

glm::ivec2 Application::getWindowSize() const {
  glm::ivec2 size;
  glfwGetWindowSize(window, &size.x, &size.y);
  return size;
}

void Application::updateCameraPosition() {
  if (scene != nullptr) {
    Player *player = scene->getPlayer();
    auto orientation = player->getTargetDirection();
    glm::vec3 cameraOffset = cameraPosition == CameraPosition::FirstPerson
                                 ? FPS_Z_OFFSET * QuatHelpers::getForward(orientation) +
                                       QuatHelpers::getUp(orientation) * FPS_Y_OFFSET
                                 : TPS_Z_OFFSET * QuatHelpers::getForward(orientation) +
                                       QuatHelpers::getUp(orientation) * TPS_Y_OFFSET;
    camera.updatePosition(player->getPosition() + cameraOffset);
    camera.setOrientation(player->getTargetDirection());
  }
}

glm::vec2 Application::calculateCrosshairOffset() {
  glm::vec2 windowSize = this->getWindowSize();
  switch (cameraPosition) {
  case CameraPosition::FirstPerson:
    return glm::vec2(0.f, -0.02f * windowSize.y);
  case CameraPosition::ThirdPerson:
    return glm::vec2(0.f, -0.06f * windowSize.y);
  }
}