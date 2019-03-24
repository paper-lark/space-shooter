#include "Spaceship.h"
#include <spdlog/spdlog.h>

Model *Spaceship::spaceshipModel = nullptr;

void Spaceship::Init() {
  SPDLOG_INFO("Loading model...");
  spaceshipModel = new Model("assets/Spaceship/Arc170.obj");
}

void Spaceship::Release() {
  if (spaceshipModel != nullptr) {
    delete spaceshipModel;
    spaceshipModel = nullptr;
    SPDLOG_INFO("Model released");
  } // TODO: be careful with releasing when models are still drawing
}