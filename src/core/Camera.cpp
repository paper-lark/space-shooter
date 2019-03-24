#include "Camera.h"
#include "Callback.h"
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

// Get camera direction
glm::vec3 Camera::getDirection() const {
  return glm::vec3(cos(glm::radians(pitch)) * cos(glm::radians(yaw)), sin(glm::radians(pitch)),
                   cos(glm::radians(pitch)) * sin(glm::radians(yaw)));
}

// Get View matrix (world -> view)
glm::mat4 Camera::getViewMatrix() const {
  glm::mat4 view = glm::lookAt(position, position + getDirection(), getUp());
  return view;
}

// updateRotation updates pitch and yaw by delta values
void Camera::updateRotation(float p, float y) {
  pitch += p;
  yaw += y;
  if (pitch > 89.f) {
    pitch = 89.f;
  } else if (pitch < -89.f) {
    pitch = -89.f;
  }
}