#include "Camera.h"
#include "../utils/QuatHelpers.h"
#include "Application.h"
#include "Callback.h"
#include <GLFW/glfw3.h>
#include <glm/gtc/matrix_transform.hpp>

// Set camera position
void Camera::updatePosition(glm::vec3 pos) {
  position = pos;
}

// Get projection matrix (view -> clip)
glm::mat4 Camera::getProjectionMatrix() const {
  glm::ivec2 windowSize = Application::getSingleton().getWindowSize();
  return glm::perspectiveFov(glm::radians(CAMERA_FOV), float(windowSize.x), float(windowSize.y), CAMERA_MIN_DEPTH, CAMERA_MAX_DEPTH);
}

// Get orientation
glm::quat Camera::getOrientation() const {
  return orientation;
}

// Get View matrix (world -> view)
glm::mat4 Camera::getViewMatrix() const {
    glm::mat4 view = glm::lookAt(position, position + QuatHelpers::getForward(orientation), QuatHelpers::getUp(orientation));
    return view;
}

// Update camera orientation
void Camera::setOrientation(glm::quat newOrientation) {
  orientation = glm::normalize(newOrientation);
}