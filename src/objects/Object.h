#ifndef SPACESHOOTER_OBJECT_H
#define SPACESHOOTER_OBJECT_H

#include "../representation/Model.h"
#include <spdlog/spdlog.h>

class Object {
  const Model *model;
  unsigned health;
  glm::vec3 position;
  float scale;
  glm::quat orientation;
  float speed = 5.05f;

public:
  // Constructor
  Object(const Model *model, unsigned health, glm::vec3 position, float scale = 1, float yaw = 0, float pitch = 0,
         float roll = 0)
      : model(model), health(health), position(position), scale(scale) {

    glm::quat qPitch = glm::angleAxis(glm::radians(pitch), glm::vec3(1, 0, 0));
    glm::quat qYaw = glm::angleAxis(glm::radians(yaw), glm::vec3(0, 1, 0));
    glm::quat qRoll = glm::angleAxis(glm::radians(roll),glm::vec3(0,0,1));
    orientation = glm::normalize(qPitch * qYaw * qRoll);
    SPDLOG_INFO("Created");
  }

  // Get model matrix for the object
  glm::mat4 getObjectModelMatrix() const;

  // Move object in the specified direction
  // TODO: use speed instead
  void Move(glm::vec3 vec);

  // Apply damage to the object. Returns true if object is still alive and false otherwise.
  bool ApplyDamage(unsigned damage) {
    if (health > damage) {
      health -= damage;
      return false;
    } else {
      health = 0;
      return true;
    }
  }

  // Set object rotation
  glm::quat rotate(glm::quat rotation);

  // Get object direction
  glm::vec3 getDirection() const;

  // Get object position
  glm::vec3 getPosition() const;

  // Get object orientation
  glm::quat getOrientation() const;

  // Get a flag whether the object is still alive
  bool IsAlive() const;

  // Draw object
  void Draw(Shader &shader) const;

  // Update object. Should be called on each frame
  void Update(float deltaTime);


  // Destructor
  ~Object() {
    SPDLOG_INFO("Destructed");
  }
};

#endif // SPACESHOOTER_OBJECT_H
