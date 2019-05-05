#ifndef SPACESHOOTER_SPACESHIP_H
#define SPACESHOOTER_SPACESHIP_H

#include "../representation/Model.h"
#include "Object.h"

#define SPACESHIP_MODEL_COUNT 3

class Spaceship : public Object {
  static Model *models;

  float timeSinceFire = 0.f;

public:
  static void init();
  static void release();

  explicit Spaceship(unsigned health, const glm::vec3 position = glm::vec3(0.f, 0.f, 0.f))
      : Object(nullptr, health, position, 2.5f, 1.f, std::make_tuple(10.0f, 10.f)) {

    if (health > 200) {
      // use first model
      model = &models[0];
      scale = 0.005f;
    } else if (health > 100) {
      // use second model
      model = &models[1];
      scale = 1.f;
    } else {
      // use third model
      model = &models[2];
      scale = .66f;
      modelRotation = glm::angleAxis(glm::radians(-90.f), glm::vec3(0, 1, 0));
    }
  }

  // Update object. Should be called on each frame
  void update(float deltaTime) override;
  void update(float deltaTime, bool shouldFire);
};

#endif // SPACESHOOTER_SPACESHIP_H
