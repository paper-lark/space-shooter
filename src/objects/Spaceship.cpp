#include "Spaceship.h"
#include <spdlog/spdlog.h>

Model *Spaceship::spaceshipModel = nullptr;

void Spaceship::init() {
  SPDLOG_INFO("Loading model...");
  spaceshipModel = new Model("assets/ARC-170/Arc170.obj");
}

void Spaceship::release() {
  if (spaceshipModel != nullptr) {
    delete spaceshipModel;
    spaceshipModel = nullptr;
    SPDLOG_INFO("Model released");
  }
}

void Spaceship::update(float deltaTime) {
  this->updateSpeed(0.5f * deltaTime);
  Object::update(deltaTime);
}