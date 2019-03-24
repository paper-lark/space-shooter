#include "Object.h"

glm::mat4 Object::getObjectModelMatrix() const {
  glm::mat4 matrix = glm::mat4(1.f);
  matrix = glm::translate(matrix, position);
  matrix = glm::rotate(matrix, glm::radians(pitch), glm::vec3(1.f, 0.f, 0.f));
  matrix = glm::rotate(matrix, glm::radians(yaw + 90),
                       glm::vec3(0.f, 1.f, 0.f)); // yaw is modified due to the model position
  matrix = glm::rotate(matrix, glm::radians(roll), glm::vec3(0.f, 0.f, 1.f));
  matrix = glm::scale(matrix, glm::vec3(scale));
  return matrix;
}

void Object::Move(glm::vec3 vec) {
  position += vec;
}

glm::vec3 Object::getDirection() const {
  return glm::vec3(cos(glm::radians(pitch)) * cos(glm::radians(yaw)), sin(glm::radians(pitch)),
                   cos(glm::radians(pitch)) * sin(glm::radians(yaw)));
}

std::tuple<float, float, float> Object::getRotation() const {
  return std::make_tuple(pitch, yaw, roll);
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
  roll += deltaTime * 10;
  this->Move(this->getDirection() * deltaTime);
}

glm::vec3 Object::getPosition() const {
  return position;
}

std::tuple<float, float, float> Object::rotate(std::tuple<float, float, float> delta) {
  pitch += std::get<0>(delta);
  yaw += std::get<1>(delta);
  roll += std::get<2>(delta);
  SPDLOG_INFO("Updated rotation: {} {} {}", pitch, yaw, roll);
  return std::make_tuple(pitch, yaw, roll);
}
