#include "Asteroid.h"

Model *Asteroid::models = nullptr;
std::default_random_engine Asteroid::generator{
    static_cast<unsigned>(std::chrono::system_clock::now().time_since_epoch().count())};

void Asteroid::init() {
  SPDLOG_INFO("Loading model...");
  models = new Model[ASTEROID_MODEL_COUNT]{Model("assets/Asteroid [1]/asteroid.obj"),
                                           Model("assets/Asteroid [2]/asteroid.obj")};
}

void Asteroid::release() {
  if (models != nullptr) {
    delete[] models;
    models = nullptr;
    SPDLOG_INFO("Model released");
  }
}