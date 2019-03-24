#ifndef SPACESHOOTER_OBJECT_H
#define SPACESHOOTER_OBJECT_H

#include "../representation/Model.h"
#include <spdlog/spdlog.h>

class Object {
  const Model *model;
  unsigned health;
  glm::vec3 position;
  float scale;
  float yaw, pitch, roll;

public:
  // Constructor
  Object(const Model *model, unsigned health, glm::vec3 position, float scale = 1, float yaw = 0, float pitch = 0,
         float roll = 0)
      : model(model), health(health), position(position), scale(scale), yaw(yaw), pitch(pitch), roll(roll) {
    SPDLOG_INFO("Created");
  }

  // Get model matrix for the object
  glm::mat4 getObjectModelMatrix() const;

  // Move object in the specified direction
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
  std::tuple<float, float, float> rotate(std::tuple<float, float, float> delta);

  // Get object rotation
  std::tuple<float, float, float> getRotation() const;

  // Get object direction
  glm::vec3 getDirection() const;

  // Get object position
  glm::vec3 getPosition() const;

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
