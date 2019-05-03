#include "Object.h"
#include <spdlog/spdlog.h>
#include "../utils/Box.h"

glm::mat4 Object::getObjectModelMatrix() const {
  glm::mat4 rotate = glm::mat4_cast(glm::normalize(orientation * modelRotation));
  glm::mat4 translate = glm::translate(glm::mat4(1.f), position);
  translate = glm::scale(translate, glm::vec3(scale));
  return translate * rotate;
}

void Object::updateSpeed(float deltaSpeed) {
  speed += deltaSpeed;
}

bool Object::isAlive() const {
  return health > 0;
}

void Object::draw(Shader &shader) const {
  shader.use();
  shader.setMatrix("model", this->getObjectModelMatrix());
  shader.setUint("health", health);
  model->draw(shader);
}

void Object::update(float deltaTime) {
  // make sure speed is in the specified range
  speed = glm::clamp(speed, std::get<0>(speedLimit), std::get<1>(speedLimit));

  // move object according to its speed
  position += speed * deltaTime * QuatHelpers::getForward(orientation);
}

glm::vec3 Object::getPosition() const {
  return position;
}

glm::quat Object::getOrientation() const {
  return orientation;
}

unsigned Object::getHealth() const {
  return health;
}

glm::quat Object::rotate(glm::quat rotation) {
  orientation = glm::normalize(orientation * rotation);
  return orientation;
}
