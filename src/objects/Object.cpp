#include "Object.h"
#include <spdlog/spdlog.h>

glm::mat4 Object::getObjectModelMatrix() const {
  glm::mat4 rotate = glm::mat4_cast(orientation);
  glm::mat4 translate = glm::translate(glm::mat4(1.f), position);
  translate = glm::scale(translate, glm::vec3(scale));
  return translate * rotate;
}

void Object::move(glm::vec3 vec) {
  SPDLOG_INFO("Moving object: {} {} {}", vec.x, vec.y, vec.z);
  position += vec * speed;
}

bool Object::isAlive() const {
  return health > 0;
}

void Object::draw(Shader &shader) const {
  shader.setMatrix("model", this->getObjectModelMatrix());
  shader.setUint("health", health);
  model->draw(shader);
}

void Object::update(float deltaTime) {
  this->rotate(glm::angleAxis(glm::radians(10.f * deltaTime), glm::vec3(0, 0, 1)));
  this->move(QuatHelpers::getForward(this->getOrientation()) * deltaTime);
}

glm::vec3 Object::getPosition() const {
  return position;
}

glm::quat Object::getOrientation() const {
  return orientation;
}

glm::quat Object::rotate(glm::quat rotation) {
  orientation = glm::normalize(orientation * rotation);
  return orientation;
}
