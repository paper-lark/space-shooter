//
// Created by Max Zhuravsky on 2019-02-17.
//

#include "Application.h"

void Application::processInput() const {
    if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
        glfwSetWindowShouldClose(this->window, true);
    }
}