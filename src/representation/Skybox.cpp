#include "Skybox.h"
#include <stb_image.h>

static float vertices[] = {-1.0f, 1.0f,  -1.0f, -1.0f, -1.0f, -1.0f, 1.0f,  -1.0f, -1.0f, 1.0f,  1.0f,  -1.0f,

                           -1.0f, -1.0f, 1.0f,  -1.0f, -1.0f, -1.0f, -1.0f, 1.0f,  -1.0f, -1.0f, 1.0f,  1.0f,

                           1.0f,  -1.0f, -1.0f, 1.0f,  -1.0f, 1.0f,  1.0f,  1.0f,  1.0f,  1.0f,  1.0f,  -1.0f,

                           -1.0f, -1.0f, 1.0f,  -1.0f, 1.0f,  1.0f,  1.0f,  1.0f,  1.0f,  1.0f,  -1.0f, 1.0f,

                           -1.0f, 1.0f,  -1.0f, 1.0f,  1.0f,  -1.0f, 1.0f,  1.0f,  1.0f,  -1.0f, 1.0f,  1.0f,

                           -1.0f, -1.0f, -1.0f, -1.0f, -1.0f, 1.0f,  1.0f,  -1.0f, -1.0f, 1.0f,  -1.0f, 1.0f};

static unsigned indices[] = {0,  1,  2,  2,  3,  0,

                             4,  5,  6,  6,  7,  4,

                             8,  9,  10, 10, 11, 8,

                             12, 13, 14, 14, 15, 12,

                             16, 17, 18, 18, 19, 16,

                             20, 21, 22, 22, 21, 23};

Skybox::Skybox(const std::vector<std::string> &faces, const Shader shader)
    : shader(shader), texture(TextureMapType::Diffuse, faces) {
  glGenVertexArrays(1, &mesh);
  glBindVertexArray(mesh);

  GLuint vbo, ebo;
  glGenBuffers(1, &vbo);
  glGenBuffers(1, &ebo);

  glBindBuffer(GL_ARRAY_BUFFER, vbo);
  glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

  // specify how to interpret vertex data from currently bound VBO
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), nullptr);
  glEnableVertexAttribArray(0);
}

void Skybox::Draw(const Camera &camera) {
  shader.use();
  glm::mat4 view = glm::mat4(glm::mat3(camera.getViewMatrix()));
  shader.setMatrix("view", view);
  shader.setMatrix("projection", camera.getProjectionMatrix());
  texture.bind(GL_TEXTURE0);
  glBindVertexArray(mesh);
  glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, nullptr);
  glBindVertexArray(0);
}
