#include "Player.h"
#include "../configuration.h"
#include "../utils/Helpers.h"
#include "Spaceship.h"
#include <spdlog/spdlog.h>

#ifdef GOD_MODE
#define MAX_TARGET_VIEW_ANGLE 360.f
#else
#define MAX_TARGET_VIEW_ANGLE 30.f
#endif

#define TILT_SPEED 30.f
#define MAX_ROLL_TILT_ANGLE 15.f
#define MAX_PITCH_TILT_ANGLE 5.f
#define MOVE_SPEED 10.f

Model *Player::spaceshipModel = nullptr;

void Player::init() {
  SPDLOG_INFO("Loading model...");
  spaceshipModel = new Model("assets/Spaceship [1]/Arc170.obj");
}

void Player::release() {
  if (spaceshipModel != nullptr) {
    delete spaceshipModel;
    spaceshipModel = nullptr;
    SPDLOG_INFO("Model released");
  }
}

void Player::rotateTarget(float pitch, float yaw) {
  targetPitch += pitch;
  targetYaw += yaw;

  // get ship orientation angles
  glm::vec3 orientationAngles = glm::eulerAngles(this->getOrientation());
  float orientationPitch = glm::degrees(orientationAngles.y);
  float orientationYaw = glm::degrees(orientationAngles.x);

  // normalize angles
  orientationPitch = std::fmod((std::fmod(orientationPitch, 360.f) + 360.f), 360.f);
  orientationYaw = std::fmod((std::fmod(orientationYaw, 360.f) + 360.f), 360.f);

  // clip camera angles
  targetPitch = glm::clamp(targetPitch, -MAX_TARGET_VIEW_ANGLE + orientationPitch,
                           MAX_TARGET_VIEW_ANGLE + orientationPitch);
  targetYaw = glm::clamp(targetYaw, -MAX_TARGET_VIEW_ANGLE + orientationYaw,
                         MAX_TARGET_VIEW_ANGLE + orientationYaw);
}

glm::quat Player::getTargetDirection() {
  return glm::quat(glm::vec3(glm::radians(targetPitch), glm::radians(targetYaw), 0.f));
}

void Player::moveInPlane(Direction dir) {
  // save direction for later update
  if (dir == Direction::Left || dir == Direction::Right) {
    xDirection = dir;
  } else {
    yDirection = dir;
  }
}

void Player::update(float deltaTime) {
  // tilt spaceship
  switch (xDirection) {
  case Direction::Right:
    deltaRoll =
        glm::clamp(deltaRoll + deltaTime * TILT_SPEED, -MAX_ROLL_TILT_ANGLE, MAX_ROLL_TILT_ANGLE);
    break;
  case Direction::Left:
    deltaRoll =
        glm::clamp(deltaRoll - deltaTime * TILT_SPEED, -MAX_ROLL_TILT_ANGLE, MAX_ROLL_TILT_ANGLE);
    break;
  case Direction::None:
    deltaRoll = deltaRoll - std::min(deltaTime * TILT_SPEED, std::abs(deltaRoll)) * sgn(deltaRoll);
    break;
  default:
    break;
  }
  switch (yDirection) {
  case Direction::Up:
    deltaPitch = glm::clamp(deltaPitch - deltaTime * TILT_SPEED, -MAX_PITCH_TILT_ANGLE,
                            MAX_PITCH_TILT_ANGLE);
    break;
  case Direction::Down:
    deltaPitch = glm::clamp(deltaPitch + deltaTime * TILT_SPEED, -MAX_PITCH_TILT_ANGLE,
                            MAX_PITCH_TILT_ANGLE);
    break;
  case Direction::None:
    deltaPitch =
        deltaPitch - std::min(deltaTime * TILT_SPEED, std::abs(deltaPitch)) * sgn(deltaPitch);
    break;
  default:
    break;
  }

  // update position
  glm::vec3 deltaPos = glm::vec3(0.f, 0.f, 0.f);
  switch (xDirection) {
  case Direction::Left:
    deltaPos.x -= deltaTime * MOVE_SPEED;
    break;
  case Direction::Right:
    deltaPos.x += deltaTime * MOVE_SPEED;
    break;
  default:
    break;
  }
  switch (yDirection) {
  case Direction::Up:
    deltaPos.y += deltaTime * MOVE_SPEED;
    break;
  case Direction::Down:
    deltaPos.y -= deltaTime * MOVE_SPEED;
    break;
  default:
    break;
  }
  position = position + deltaPos;

  // Trigger underlying object update
  Object::update(deltaTime);

  // reset player direction for next cycle
  xDirection = Direction::None;
  yDirection = Direction::None;
}