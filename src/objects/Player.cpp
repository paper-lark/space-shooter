#include "Player.h"
#include "Spaceship.h"
#include <spdlog/spdlog.h>

Model *Player::spaceshipModel = nullptr;

void Player::Init() {
  SPDLOG_INFO("Loading model...");
  spaceshipModel = new Model("assets/ARC-170/Arc170.obj");
}

void Player::Release() {
  if (spaceshipModel != nullptr) {
    delete spaceshipModel;
    spaceshipModel = nullptr;
    SPDLOG_INFO("Model released");
  } // TODO: be careful with releasing when models are still drawing
}