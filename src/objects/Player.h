#ifndef SPACESHOOTER_PLAYER_H
#define SPACESHOOTER_PLAYER_H

#include "../representation/Model.h"
#include "../utils/Direction.h"
#include "Object.h"

class Player : public Object {
  static Model *spaceshipModel;
  float targetPitch;
  float targetYaw;

  // Current cycle player movement direction
  Direction xDirection = Direction::None;
  Direction yDirection = Direction::None;

public:
  static void init();

  static void release();

  // Constructor
  explicit Player(unsigned health, const glm::vec3 position, float yaw = 0.f, float pitch = 0.f,
         float roll = 0.f)
      : Object(spaceshipModel, health, position, 2.5f, 0.005f, std::make_tuple(0.f, 0.f),
               glm::quat(1.f, 0.f, 0.f, 0.f), yaw, pitch, roll),
        targetPitch(pitch), targetYaw(yaw) {}

  // Rotate camera
  void rotateTarget(float pitch, float yaw);

  // Get player target (camera) direction
  glm::quat getTargetDirection();

  // Trigger player movement in the next game cycle
  void moveInPlane(Direction dir);

  // Update player. Should be called on each frame
  void update(float deltaTime) override;
};

#endif // SPACESHOOTER_PLAYER_H
