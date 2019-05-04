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
                 0.0032,
                 0.0016};
  }

  Star(unsigned health, const glm::vec3 position)
      : Object(starModel, health, position, 0.f, 100.f, std::make_tuple(3.f, 3.f)) {}
};

#endif // SPACESHOOTER_STAR_H
