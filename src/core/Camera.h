#ifndef MAIN_CAMERA_H
#define MAIN_CAMERA_H

#include <glm/glm.hpp>


class Camera {
    glm::vec3 position = glm::vec3(-3.0f, 0.5f, 0.0f);
    float pitch = 0.f; // camera pitch in deg
    float yaw = 0.f; // camera yaw in deg
    float fov = 45.f; // camera field of view
    const float speed = 0.5f; // camera movement speed

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

    // Get camera position
    glm::vec3 getPos() const {
        return glm::vec3(position);
    }

    // Update camera rotation
    void updateRotation(float deltaYaw, float deltaPitch);
};


#endif //MAIN_CAMERA_H
