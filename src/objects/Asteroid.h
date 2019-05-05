#ifndef SPACESHOOTER_ASTEROID_H
#define SPACESHOOTER_ASTEROID_H

#include "Object.h"
#include <chrono>
#include <random>

#define ASTEROID_MODEL_COUNT 2

class Asteroid : public Object {
  static Model *models;
  static std::default_random_engine generator;

public:
  static void init();
  static void release();

  explicit Asteroid(unsigned health, const glm::vec3 position = glm::vec3(0.f, 0.f, 0.f))
      : Object(nullptr, health, position, 2.5f, 1.f, std::make_tuple(10.0f, 10.f)) {

    std::uniform_int_distribution<unsigned> modelDistribution(0, ASTEROID_MODEL_COUNT - 1);
    unsigned modelNumber = modelDistribution(generator);
    model = &models[modelNumber];

    switch (modelNumber) {
    case 0:
      scale = 0.15f;
      break;
    case 1:
      scale = 2.25f;
      break;
    default:
      throw std::runtime_error("Unknown asteroid model number");
    }
  }
};

#endif // SPACESHOOTER_ASTEROID_H
