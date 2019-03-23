#ifndef SPACESHOOTER_SKYBOX_H
#define SPACESHOOTER_SKYBOX_H

#include <vector>
#include <string>
#include "Shader.h"
#include "../core/Camera.h"
#include "Texture.h"
#include "Mesh.h"

class Skybox {
    Shader shader;
    Texture texture;
    GLuint mesh;

public:
    explicit Skybox(const std::vector<std::string> &faces, const Shader shader);

    void Draw(const Camera &camera);
};


#endif //SPACESHOOTER_SKYBOX_H
