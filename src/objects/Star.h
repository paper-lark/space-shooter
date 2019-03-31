#ifndef SPACESHOOTER_STAR_H
#define SPACESHOOTER_STAR_H

#include "Object.h"

struct Light {
  glm::vec3 ambient;
  glm::vec3 diffuse;
  glm::vec3 specular;

  float constant;
  float linear;
  float quadratic;
};

class Star : public Object {
  static Model *starModel;

public:
  static void init();

  static void release();

  static Light getLight() {
    return Light{glm::vec3(0.2f, 0.2f, 0.2f),
                 glm::vec3(0.5f, 0.5f, 0.5f),
                 glm::vec3(1.0f, 1.0f, 1.0f),

                 1.0,
                 0.09,
                 0.032};
  }

  Star(unsigned health, const glm::vec3 position)
      : Object(starModel, health, position, glm::vec3(.5f, .5f, .5f), 30.f, std::make_tuple(0.f, 0.f)) {}
};

#endif // SPACESHOOTER_STAR_H
