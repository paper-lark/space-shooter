#include "Object.h"

glm::mat4 Object::getObjectModelMatrix() const {
  glm::mat4 matrix = glm::mat4(1.f);
  matrix = glm::translate(matrix, position);
  matrix = glm::rotate(matrix, glm::radians(pitch), glm::vec3(1.f, 0.f, 0.f));
  matrix = glm::rotate(matrix, glm::radians(yaw), glm::vec3(0.f, 1.f, 0.f));
  matrix = glm::rotate(matrix, glm::radians(roll), glm::vec3(0.f, 0.f, 1.f));
  matrix = glm::scale(matrix, glm::vec3(scale));
  return matrix;
}

void Object::Move(glm::vec3 vec) {
  position += vec;
}

glm::vec3 Object::getDirection() const {
  return glm::vec3(cos(glm::radians(pitch)) * sin(glm::radians(yaw)), sin(glm::radians(pitch)),
                   cos(glm::radians(pitch)) * cos(glm::radians(yaw)));
  // yaw value used here is +90 because of the spaceship
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
  this->ApplyDamage(1);
  this->Move(this->getDirection() * deltaTime);
}