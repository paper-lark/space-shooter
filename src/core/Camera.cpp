#include <glm/gtc/matrix_transform.hpp>
#include <GLFW/glfw3.h>
#include "Camera.h"
#include "Callback.h"

// Move camera
void Camera::move(glm::vec3 direction) {
    position += direction * speed;
}

// Get projection matrix (view -> clip)
glm::mat4 Camera::getProjectionMatrix() const {
    return glm::perspective(
            glm::radians(fov),
            float(Callback::windowSize[0]) / Callback::windowSize[1],
            0.1f,
            100.0f
    );
}

// Get camera direction
glm::vec3 Camera::getDirection() const {
    return glm::vec3(
            cos(glm::radians(pitch)) * cos(glm::radians(yaw)),
            sin(glm::radians(pitch)),
            cos(glm::radians(pitch)) * sin(glm::radians(yaw))
    );
}


// Get View matrix (world -> view)
glm::mat4 Camera::getViewMatrix() const {
    glm::mat4 view = glm::lookAt(position, position + getDirection(), getUp());
    return view;
}

// updateRotation updates pitch and yaw by specified delta values
void Camera::updateRotation(float deltaYaw, float deltaPitch) {
    pitch += deltaPitch;
    yaw += deltaYaw;
    if (pitch > 89.f) {
        pitch = 89.f;
    } else if (pitch < -89.f) {
        pitch = -89.f;
    }
}