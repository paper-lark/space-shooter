#ifndef SPACESHOOTER_APPLICATION_H
#define SPACESHOOTER_APPLICATION_H

#define GL_SILENCE_DEPRECATION
#include "../objects/Player.h"
#include "Camera.h"
#include "CameraPosition.h"
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

class Application {
private:
  GLFWwindow *window;
  double previousMoment = 0.0;
  double deltaTime = 0.0;
  Player *player = nullptr;
  const float sensitivity = 0.05f; // mouse sensitivity
  CameraPosition cameraPosition = CameraPosition::ThirdPerson;
  static Application &instance;
  unsigned score = 0;

public:
  // Application camera object
  Camera camera;

  // Constructor
  Application(GLFWwindow *window = nullptr) : window(window) {}

  // Process keyboard input
  void processKeyboardInput();

  // Process mouse input
  void processMouseInput(GLFWwindow *window, double posX, double posY);

  // Update application state
  void update();

  // Get delta time from previous render
  float getDeltaTime() const;

  // Get score
  unsigned getScore() const;

  // Bind player to the application
  void bindPlayer(Player *p);

  // Get window size
  glm::ivec2 getWindowSize() const;

  // Initialize application singleton
  static void initialize(GLFWwindow *window) {
    Application::instance.window = window;
  }

  // Get application singleton
  static Application &getSingleton() {
    return Application::instance;
  }
};

#endif // SPACESHOOTER_APPLICATION_H
