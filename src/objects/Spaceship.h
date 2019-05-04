#ifndef SPACESHOOTER_SPACESHIP_H
#define SPACESHOOTER_SPACESHIP_H

#include "../representation/Model.h"
#include "Object.h"

class Spaceship : public Object {
  static Model *spaceshipModel;
  float timeSinceFire = 0.f;

public:
  static void init();

  static void release();

  Spaceship(unsigned health, const glm::vec3 position = glm::vec3(0.f, 0.f, 0.f))
      : Object(spaceshipModel, health, position, 3.f, 0.005f, std::make_tuple(10.0f, 10.f)) {}

  // Update object. Should be called on each frame
  void update(float deltaTime) override;
  void update(float deltaTime, bool shouldFire);
};

#endif // SPACESHOOTER_SPACESHIP_H
