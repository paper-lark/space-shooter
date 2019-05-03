#ifndef SPACESHOOTER_TORPEDO_H
#define SPACESHOOTER_TORPEDO_H

#include "../representation/Model.h"
#include "Object.h"
#include <glm/glm.hpp>

class Torpedo : public Object {
  static Model *torpedoModel;

public:
  static void init();

  static void release();

  Torpedo(unsigned health, const glm::vec3 position)
      : Object(torpedoModel, health, position, 0.1f, 0.1f, std::make_tuple(15.f, 20.f), glm::angleAxis(glm::radians(90.f), glm::vec3(0, 1, 0))) {}

  Torpedo(unsigned health, const glm::vec3 position, glm::quat orientation)
      : Object(torpedoModel, health, position, 0.1f, 0.1f, std::make_tuple(15.f, 20.f), orientation, glm::angleAxis(glm::radians(90.f), glm::vec3(0, 1, 0))) {}

  // Update object. Should be called on each frame
  void update(float deltaTime) override;
};

#endif // SPACESHOOTER_TORPEDO_H
