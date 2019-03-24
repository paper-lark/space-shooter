#ifndef SPACESHOOTER_PLAYER_H
#define SPACESHOOTER_PLAYER_H

#include "../representation/Model.h"
#include "Object.h"

class Player : public Object {
  static Model *spaceshipModel;

public:
  static void init();

  static void release();

  Player(unsigned health, const glm::vec3 position) : Object(spaceshipModel, health, position, 0.005f, std::make_tuple(0.1f, 5.f)) {}
};

#endif // SPACESHOOTER_PLAYER_H
