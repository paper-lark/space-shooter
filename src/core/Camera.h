#ifndef SPACESHOOTER_CAMERA_H
#define SPACESHOOTER_CAMERA_H

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

class Camera {
  glm::vec3 position = glm::vec3(0.f, 1.f, 0.f);
  glm::quat orientation = glm::normalize(glm::angleAxis(0.f, glm::vec3(1, 0, 0)) * glm::angleAxis(0.f, glm::vec3(0, 1, 0)) * glm::angleAxis(0.f, glm::vec3(0, 0, 1)));
  float fov = 45.f;         // camera field of view in deg
  const float speed = 0.5f; // camera movement speed

public:

  // Set camera position
  // TODO: remove `move()`
  void updatePosition(glm::vec3 pos);

  // Move camera
  void move(glm::vec3 direction);

  // Get View matrix (world -> view)
  glm::mat4 getViewMatrix() const;

  // Get projection matrix (view -> clip)
  glm::mat4 getProjectionMatrix() const;

  // Get camera direction
  glm::vec3 getDirection() const;

  // Get camera upward direction
  glm::vec3 getUp() const;

  // Get camera position
  glm::vec3 getPos() const {
    return glm::vec3(position);
  }

  // Update camera orientation
  void setOrientation(glm::quat orientation);
};

#endif // SPACESHOOTER_CAMERA_H
