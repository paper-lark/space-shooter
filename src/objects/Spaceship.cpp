#include "Spaceship.h"
#include "../core/Application.h"
#include <spdlog/spdlog.h>

#define FIRE_COOLDOWN 3.f

Model *Spaceship::models = nullptr;

void Spaceship::init() {
  SPDLOG_INFO("Loading model...");
  models = new Model[SPACESHIP_MODEL_COUNT]{
      Model("assets/Spaceship [1]/Arc170.obj"),
      Model("assets/Spaceship [2]/f.obj"),
      Model("assets/Spaceship [3]/SS1.obj"),
  };
}

void Spaceship::release() {
  if (models != nullptr) {
    delete [] models;
    models = nullptr;
    SPDLOG_INFO("Model released");
  }
}

void Spaceship::update(float deltaTime) {
  this->update(deltaTime, false);
}

void Spaceship::update(float deltaTime, bool shouldFire) {
  timeSinceFire += deltaTime;
  Object::update(deltaTime);

  if (shouldFire && timeSinceFire > FIRE_COOLDOWN) {
    SPDLOG_INFO("Firing at {} {} {}", position.x, position.y, position.z);
    auto scene = Application::getSingleton().getScene();
    if (scene != nullptr) {
      scene->createTorpedo(position, orientation);
    }
    timeSinceFire = 0;
  }
}