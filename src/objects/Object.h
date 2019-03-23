#ifndef SPACESHOOTER_OBJECT_H
#define SPACESHOOTER_OBJECT_H

#include <spdlog/spdlog.h>
#include "../representation/Model.h"

class Object {
    const Model *model;
    unsigned health;
    glm::vec3 position;
    float scale;
    float yaw, pitch, roll;

    // Get model matrix for the object
    glm::mat4 getObjectModelMatrix() const;

public:
    // Constructor
    Object(const Model *model, unsigned health, glm::vec3 position, float scale = 1, float yaw = 0, float pitch = 0, float roll = 0)
        : model(model), health(health), position(position), scale(scale), yaw(yaw), pitch(pitch), roll(roll) {
        SPDLOG_INFO("Created");
    }

    // Move object in the specified direction
    void Move(glm::vec3 vec);

    // Apply damage to the object. Returns true if object is still alive and false otherwise.
    bool ApplyDamage(unsigned damage) {
        if (health > damage) {
            health -= damage;
            return false;
        } else {
            health = 0;
            return true;
        }
    }

    // Get object direction
    glm::vec3 getDirection() const;

    // Get a flag whether the object is still alive
    bool IsAlive() const;

    // Draw object
    void Draw(Shader &shader) const;

    // Update object. Should be called on each frame
    void Update(float deltaTime);

    // Destructor
    ~Object() {
        SPDLOG_INFO("Destructed");
    }
};


#endif //SPACESHOOTER_OBJECT_H
