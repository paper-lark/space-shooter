#include "Star.h"
#include <spdlog/spdlog.h>

Model *Star::starModel = nullptr;

void Star::init() {
  SPDLOG_INFO("Loading model...");
  starModel = new Model("assets/Star [1]/Mercury 1K.obj");
}

void Star::release() {
  if (starModel != nullptr) {
    delete starModel;
    starModel = nullptr;
    SPDLOG_INFO("Model released");
  }
}