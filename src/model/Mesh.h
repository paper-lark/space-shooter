#ifndef SPACESHOOTER_MESH_H
#define SPACESHOOTER_MESH_H

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glad/glad.h>
#include <vector>
#include "Texture.h"
#include "../utils/Shader.h"

using std::vector;

struct Vertex {
    glm::vec3 position;
    glm::vec3 normal;
    glm::vec2 textureCoords;

    Vertex(glm::vec3 position, glm::vec3 normal, glm::vec2 textureCoords):
        position(position), normal(normal), textureCoords(textureCoords) {}
};


class Mesh {
    GLuint vao = 0, vbo = 0, ebo = 0;
    void setupMesh();
public:
    const vector<Vertex> vertices;
    const vector<unsigned> indices;
    const vector<Texture> textures;

    Mesh(vector<Vertex> &vertices, vector<unsigned> &indices, vector<Texture> &textures) :
        vertices(vertices), indices(indices), textures(textures) {
        setupMesh();
    };

    void Draw(Shader shader) const;

// TODO: fix
//    ~Mesh() {
//        glDeleteVertexArrays(1, &vao);
//        glDeleteBuffers(1, &vbo);
//        glDeleteBuffers(1, &ebo);
//    }
};


#endif //SPACESHOOTER_MESH_H
