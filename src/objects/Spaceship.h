#ifndef SPACESHOOTER_SPACESHIP_H
#define SPACESHOOTER_SPACESHIP_H

#include "../representation/Model.h"
#include "Object.h"

class Spaceship : public Object {
  static Model *spaceshipModel;

public:
  static void init();

  static void release();

  Spaceship(unsigned health, const glm::vec3 position)
      : Object(spaceshipModel, health, position, 3.f, 0.005f, std::make_tuple(0.f, 0.f)) {} // TODO: std::make_tuple(0.1f, 3.f)

  // Update object. Should be called on each frame
  void update(float deltaTime) override;
};

#endif // SPACESHOOTER_SPACESHIP_H
