#include "Callback.h"
#include <iostream>
#include <spdlog/spdlog.h>

void Callback::error(int code, const char *description) {
  SPDLOG_ERROR("Error occurred: {} [{}]", description, code);
}