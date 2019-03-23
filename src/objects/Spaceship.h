#ifndef SPACESHOOTER_SPACESHIP_H
#define SPACESHOOTER_SPACESHIP_H


#include "Object.h"
#include "../representation/Model.h"

class Spaceship: public Object {
    static Model *spaceshipModel;

public:
    static void Init();

    static void Release();

    Spaceship(unsigned health, const glm::vec3 position): Object(spaceshipModel, health, position) {}
};


#endif //SPACESHOOTER_SPACESHIP_H
