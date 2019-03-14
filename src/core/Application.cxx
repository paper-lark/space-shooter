#include "Application.h"

void Application::processInput() const {
    if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
        glfwSetWindowShouldClose(this->window, true);
    }
    if(glfwGetKey(window, GLFW_KEY_1) == GLFW_PRESS) {
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    }
    if(glfwGetKey(window, GLFW_KEY_2) == GLFW_PRESS) {
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    }
}


// Get projection matrix (view -> clip)
glm::mat4 Application::getProjectionMatrix() const {
    return glm::perspective(
            glm::radians(45.0f),
            float(Callback::windowSize[0]) / Callback::windowSize[1],
            0.1f,
            100.0f
    );
}


// Get View matrix (world -> view)
glm::mat4 Application::getViewMatrix() const {
    return glm::translate(glm::mat4(1.f), glm::vec3(0.0f, 0.0f, -3.0f));
}


// Get time passed from previous render
double Application::getDeltaTime() const {
    static double previous = glfwGetTime();
    double result = glfwGetTime() - previous;
    previous += result;
    return result;
}