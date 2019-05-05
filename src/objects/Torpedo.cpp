#include "Torpedo.h"
#include <spdlog/spdlog.h>

Model *Torpedo::torpedoModel = nullptr;

void Torpedo::init() {
  SPDLOG_INFO("Loading model...");
  torpedoModel = new Model("assets/Torpedo [1]/missile.obj");
}

void Torpedo::release() {
  if (torpedoModel != nullptr) {
    delete torpedoModel;
    torpedoModel = nullptr;
    SPDLOG_INFO("Model released");
  }
}

void Torpedo::update(float deltaTime) {
  this->updateSpeed(1.f * deltaTime);
  Object::update(deltaTime);
}