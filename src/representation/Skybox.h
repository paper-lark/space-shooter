#ifndef SPACESHOOTER_SKYBOX_H
#define SPACESHOOTER_SKYBOX_H

#include "../core/Camera.h"
#include "Mesh.h"
#include "Shader.h"
#include "Texture.h"
#include <string>
#include <vector>

class Skybox {
  Shader shader;
  Texture texture;
  GLuint mesh;

public:
  explicit Skybox(const std::vector<std::string> &faces, const Shader shader);

  void draw(const Camera &camera);
};

#endif // SPACESHOOTER_SKYBOX_H
