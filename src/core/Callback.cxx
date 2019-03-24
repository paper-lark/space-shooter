#include "Callback.h"
#include <iostream>
#include <spdlog/spdlog.h>

GLint Callback::windowSize[2] = {1, 1};

void Callback::windowResize(GLFWwindow *, GLint width, GLint height) {
  Callback::windowSize[0] = width;
  Callback::windowSize[1] = height;
  glViewport(0, 0, width, height);
}

void Callback::error(int code, const char *description) {
  SPDLOG_ERROR("Error occurred: {} [{}]", description, code);
}