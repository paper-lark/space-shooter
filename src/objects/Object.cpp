#include "Object.h"
#include <spdlog/spdlog.h>

glm::mat4 Object::getObjectModelMatrix() const {
  glm::mat4 rotate = glm::mat4_cast(orientation);
  glm::mat4 translate = glm::translate(glm::mat4(1.f), position);
  translate = glm::scale(translate, glm::vec3(scale));
  return translate * rotate;
}

void Object::Move(glm::vec3 vec) {
  SPDLOG_INFO("Moving object: {} {} {}", vec.x, vec.y, vec.z);
  position += vec * speed;
}

glm::vec3 Object::getDirection() const {
  auto dir = glm::normalize(glm::vec3(2 * (orientation.x * orientation.z + orientation.w * orientation.y),
                                  2 * (orientation.y * orientation.z - orientation.w * orientation.x),
                                  1 - 2 * (orientation.x * orientation.x + orientation.y * orientation.y)));
  return dir;
}

bool Object::IsAlive() const {
  return health > 0;
}

void Object::Draw(Shader &shader) const {
  shader.setMatrix("model", this->getObjectModelMatrix());
  shader.setUint("health", health);
  model->Draw(shader);
}

void Object::Update(float deltaTime) {
  this->rotate(glm::angleAxis(glm::radians(10.f * deltaTime), glm::vec3(0, 0, 1)));
  this->Move(this->getDirection() * deltaTime);
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
