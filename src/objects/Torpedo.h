#ifndef SPACESHOOTER_TORPEDO_H
#define SPACESHOOTER_TORPEDO_H

#include "../representation/Model.h"
#include "Object.h"
#include <glm/glm.hpp>

#define TORPEDO_SCALE 0.15f
#define TORPEDO_BBOX_RADIUS 0.15f

class Torpedo : public Object {
  static Model *torpedoModel;

public:
  static void init();

  static void release();

  Torpedo(unsigned health, const glm::vec3 position)
      : Object(torpedoModel, health, position, TORPEDO_BBOX_RADIUS, TORPEDO_SCALE,
               std::make_tuple(30.f, 60.f),
               glm::angleAxis(glm::radians(90.f), glm::vec3(0, 1, 0))) {}

  Torpedo(unsigned health, const glm::vec3 position, glm::quat orientation)
      : Object(torpedoModel, health, position, TORPEDO_BBOX_RADIUS, TORPEDO_SCALE,
               std::make_tuple(30.f, 60.f), orientation, glm::quat(1.f, 0.f, 0.f, 0.f)) {}

  // Update object. Should be called on each frame
  void update(float deltaTime) override;
};

#endif // SPACESHOOTER_TORPEDO_H
