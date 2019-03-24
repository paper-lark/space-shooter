#ifndef SPACESHOOTER_SPACESHIP_H
#define SPACESHOOTER_SPACESHIP_H

#include "../representation/Model.h"
#include "Object.h"

class Spaceship : public Object {
  static Model *spaceshipModel;

public:
  static void init();

  static void release();

  Spaceship(unsigned health, const glm::vec3 position) : Object(spaceshipModel, health, position, 0.005f) {}
};

#endif // SPACESHOOTER_SPACESHIP_H
