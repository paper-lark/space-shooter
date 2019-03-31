#ifndef SPACESHOOTER_MESH_H
#define SPACESHOOTER_MESH_H

#include "Shader.h"
#include "Texture.h"
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <vector>

using std::vector;

struct Vertex {
  glm::vec3 position;
  glm::vec3 normal;
  glm::vec2 textureCoords;

  Vertex(glm::vec3 position, glm::vec3 normal, glm::vec2 textureCoords)
      : position(position), normal(normal), textureCoords(textureCoords) {}
};

class Mesh {
  GLuint vao = 0, vbo = 0, ebo = 0;
  void setupMesh();

public:
  const vector<Vertex> vertices;
  const vector<unsigned> indices;
  const vector<Texture> textures;

  Mesh(vector<Vertex> &vertices, vector<unsigned> &indices, vector<Texture> &textures)
      : vertices(vertices), indices(indices), textures(textures) {
    setupMesh();
  };
  Mesh(const Mesh &) = delete;

  Mesh(Mesh &&other)
      : vao(other.vao), vbo(other.vbo), ebo(other.ebo), vertices(other.vertices), indices(other.indices),
        textures(other.textures) {
    other.ebo = 0;
    other.vao = 0;
    other.vbo = 0;
  }

  void draw(Shader shader) const;

  ~Mesh();
};

#endif // SPACESHOOTER_MESH_H
