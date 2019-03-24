#include "Camera.h"
#include "Callback.h"
#include "../utils/QuatHelpers.h"
#include <GLFW/glfw3.h>
#include <glm/gtc/matrix_transform.hpp>

// Move camera
void Camera::move(glm::vec3 direction) {
  position += direction * speed;
}

// Set camera position
void Camera::updatePosition(glm::vec3 pos) {
  position = pos;
}

// Get projection matrix (view -> clip)
glm::mat4 Camera::getProjectionMatrix() const {
  return glm::perspective(glm::radians(fov), float(Callback::windowSize[0]) / Callback::windowSize[1], 0.1f, 250.0f);
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