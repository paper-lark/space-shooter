#ifndef MAIN_CAMERA_H
#define MAIN_CAMERA_H

#include <glm/glm.hpp>


class Camera {
    mutable glm::vec3 position = glm::vec3(0.0f, 0.0f, 3.0f); // TODO: remove mutability
    float pitch = 0.f; // camera pitch in deg
    float yaw = 0.f; // camera yaw in deg
    const float speed = 0.5f;

public:
    // Move camera
    void move(glm::vec3 direction);

    // Get View matrix (world -> view)
    glm::mat4 getViewMatrix() const;

    // Get projection matrix (view -> clip)
    glm::mat4 getProjectionMatrix() const;

    // Get camera direction
    glm::vec3 getDirection() const;

    // Get camera upward direction
    glm::vec3 getUp() const {
        return glm::vec3(0.f, 1.f, 0.f);
    }
};


#endif //MAIN_CAMERA_H
