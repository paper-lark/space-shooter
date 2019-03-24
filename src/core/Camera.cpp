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

glm::vec3 Camera::getUp() const {
  return glm::normalize(glm::vec3(2 * (orientation.x * orientation.y - orientation.w * orientation.z),
                                  1 - 2 * (orientation.x * orientation.x + orientation.z * orientation.z),
                                  2 * (orientation.y * orientation.z + orientation.w * orientation.x)));
}

// Get camera direction
// Read more: https://www.gamedev.net/forums/topic/56471-extracting-direction-vectors-from-quaternion/
glm::vec3 Camera::getDirection() const {
  auto dir = glm::normalize(glm::vec3(2 * (orientation.x * orientation.z + orientation.w * orientation.y),
                                  2 * (orientation.y * orientation.z - orientation.w * orientation.x),
                                  1 - 2 * (orientation.x * orientation.x + orientation.y * orientation.y)));
  return dir;
}

// Get View matrix (world -> view)
glm::mat4 Camera::getViewMatrix() const {
//  glm::mat4 rotate = glm::mat4_cast(orientation);
//  glm::mat4 translate = glm::mat4(1.0f);
//  translate = glm::translate(translate, -position);
//
//  return rotate * translate;
    glm::mat4 view = glm::lookAt(position, position + this->getDirection(), getUp());
    return view;
}

// Update camera orientation
void Camera::setOrientation(glm::quat newOrientation) {
  orientation = glm::normalize(newOrientation);
}