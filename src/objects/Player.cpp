#include "Player.h"
#include "Spaceship.h"
#include <spdlog/spdlog.h>

Model *Player::spaceshipModel = nullptr;

void Player::init() {
  SPDLOG_INFO("Loading model...");
  spaceshipModel = new Model("assets/ARC-170/Arc170.obj");
}

void Player::release() {
  if (spaceshipModel != nullptr) {
    delete spaceshipModel;
    spaceshipModel = nullptr;
    SPDLOG_INFO("Model released");
  } // TODO: be careful with releasing when models are still drawing
}