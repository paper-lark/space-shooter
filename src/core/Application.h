#ifndef SPACESHOOTER_APPLICATION_H
#define SPACESHOOTER_APPLICATION_H

#include "../objects/Player.h"
#include "Camera.h"
#include "CameraPosition.h"
#include "Scene.h"
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

#define MOUSE_SENSITIVITY 0.075f
#define PLAYER_FIRE_COOLDOWN 1.f

class Application {
private:
  GLFWwindow *window;
  double previousMoment = 0.0;
  double deltaTime = 0.0;
  Scene *scene = nullptr;
  double lastFireTime = 0;
  CameraPosition cameraPosition = CameraPosition::ThirdPerson;
  static Application &instance;
  unsigned score = 0;


  // Update camera position depending on the camera mode
  void updateCameraPosition();

public:
  // Application camera object
  Camera camera;

  // Constructor
  explicit Application(GLFWwindow *window = nullptr) : window(window) {}

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

  // Update score
  void updateScore(unsigned delta);

  // Bind scene to the application
  void bindScene(Scene *p);

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

  // Get scene
  Scene *getScene() const {
    return scene;
  }

  // Calculate crosshair offset from the center of the HUD depending on the camera mode
  glm::vec2 calculateCrosshairOffset();
};

#endif // SPACESHOOTER_APPLICATION_H
