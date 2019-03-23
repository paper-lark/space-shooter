#ifndef SPACESHOOTER_OBJECT_H
#define SPACESHOOTER_OBJECT_H

#include <spdlog/spdlog.h>
#include "../representation/Model.h"

class Object {
    const Model *model;
    unsigned health;
    glm::vec3 position;

    glm::mat4 getObjectModelMatrix() const {
        glm::mat4 matrix = glm::mat4(1.f);
        matrix = glm::translate(matrix, position);
        matrix = glm::scale(matrix, glm::vec3(0.005f, 0.005f, 0.005f));
        return matrix; // TODO: move scaling somewhere else
    }

public:
    Object(const Model *model, unsigned health, const glm::vec3 position): model(model), health(health), position(position) {
        SPDLOG_INFO("Created");
    }

    void Move(const glm::vec3 vec) {
        position += vec;
    }

    bool ApplyDamage(unsigned damage) {
        if (health > damage) {
            health -= damage;
            return false;
        } else {
            health = 0;
            return true;
        }
    }

    bool IsAlive() const {
        return health > 0;
    }

    void Draw(Shader &shader) const {
        shader.setMatrix("model", this->getObjectModelMatrix());
        model->Draw(shader);
    }

    ~Object() {
        SPDLOG_INFO("Destructed");
    }
};


#endif //SPACESHOOTER_OBJECT_H
