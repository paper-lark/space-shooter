#ifndef SPACESHOOTER_OBJECT_H
#define SPACESHOOTER_OBJECT_H

#include "../representation/Model.h"
#include "../utils/QuatHelpers.h"
#include <spdlog/spdlog.h>

class Object {
protected:
  const Model *model;
  unsigned health;
  glm::vec3 position;
  float bboxSize;
  float scale;
  glm::quat orientation;
  float speed = 0.f;
  const glm::quat modelRotation;
  std::tuple<float, float> speedLimit;

  // Orientation deltas used in pseudo-object space
  float deltaPitch = 0.f;
  float deltaYaw = 0.f;
  float deltaRoll = 0.f;

public:
  // Constructor
  Object(const Model *model, unsigned health, glm::vec3 position, float bboxSize, float scale,
         std::tuple<float, float> speedLimit, glm::quat modelRotation = glm::quat(1.f, 0.f, 0.f, 0.f),
         float yaw = 0, float pitch = 0, float roll = 0)
      : model(model), health(health), position(position), bboxSize(bboxSize), scale(scale), orientation(),
        modelRotation(modelRotation), speedLimit(std::move(speedLimit)) {

    glm::quat qPitch = glm::angleAxis(glm::radians(pitch), glm::vec3(1, 0, 0));
    glm::quat qYaw = glm::angleAxis(glm::radians(yaw), glm::vec3(0, 1, 0));
    glm::quat qRoll = glm::angleAxis(glm::radians(roll), glm::vec3(0, 0, 1));
    orientation = glm::normalize(qPitch * qYaw * qRoll);
    SPDLOG_INFO("Created");
  }

  // Constructor
  Object(const Model *model, unsigned health, glm::vec3 position, float bboxSize, float scale,
         std::tuple<float, float> speedLimit, glm::quat orientation, glm::quat modelRotation)
      : model(model), health(health), position(position), bboxSize(bboxSize), scale(scale), orientation(orientation),
        modelRotation(modelRotation), speedLimit(std::move(speedLimit)) {
    SPDLOG_INFO("Created");
  }

  // Get model matrix for the object
  glm::mat4 getObjectModelMatrix() const;

  // Apply damage to the object. Returns true if object is still alive and false otherwise.
  bool applyDamage(unsigned damage) {
    if (health > damage) {
      health -= damage;
      return false;
    } else {
      health = 0;
      return true;
    }
  }

  // Update speed by a specified delta
  // TODO: incapsulate acceleration
  void updateSpeed(float deltaSpeed);

  // Set object rotation
  glm::quat rotate(glm::quat rotation);

  // Get object position
  glm::vec3 getPosition() const;

  // Set object position
  void setPosition(glm::vec3 aPosition);

  // Get object orientation
  glm::quat getOrientation() const;

  // Set object orientation
  void setOrientation(glm::quat anOrientation);

  // Get health
  unsigned getHealth() const;

  // Get a flag whether the object is still alive
  bool isAlive() const;

  // Draw object
  void draw(Shader &shader) const;

  // Get bounding box
  glm::vec4 getBBox() {
    return glm::vec4(position, bboxSize); // TODO: should be centered?
  }

  // Update object. Should be called on each frame
  virtual void update(float deltaTime);

  // Destructor
  virtual ~Object() {
    SPDLOG_INFO("Destructed");
  }
};

#endif // SPACESHOOTER_OBJECT_H
