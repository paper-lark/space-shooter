#include <iostream>
#include "Callback.h"

GLint Callback::windowSize[2] = {1, 1};

void Callback::windowResize(GLFWwindow *_, GLint width, GLint height) {
    Callback::windowSize[0] = width;
    Callback::windowSize[1] = height;
    glViewport(0, 0, width, height);
}

void Callback::error(int code, const char *description) {
    std::cerr << "Error occurred [" << code << "]: " << description << std::endl;
}