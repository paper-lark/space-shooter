//
// Created by Max Zhuravsky on 2019-02-17.
//

#include <iostream>
#include "Callback.h"


void Callback::windowResize(GLFWwindow *window, GLint width, GLint height) {
    glViewport(0, 0, width, height);
}

void Callback::error(int code, const char *description) {
    std::cerr << "Error occured [" << code << "]: " << description << std::endl;
}