#ifndef SPACESHOOTER_BOX_H
#define SPACESHOOTER_BOX_H

#include <glm/glm.hpp>
#include "../representation/Shader.h"

class Box {
public:
  static void DrawBox(Shader &shader, glm::vec3 size) {
    float vertices[] = {
         size.x / 2,  size.y / 2, -size.z / 2,
         size.x / 2, -size.y / 2, -size.z / 2,
        -size.x / 2, -size.y / 2, -size.z / 2,
        -size.x / 2,  size.y / 2, -size.z / 2,

         size.x / 2,  size.y / 2, size.z / 2,
         size.x / 2, -size.y / 2, size.z / 2,
        -size.x / 2, -size.y / 2, size.z / 2,
        -size.x / 2,  size.y / 2, size.z / 2,
    };
    unsigned int indices[] = {
        0, 1, 3,
        1, 2, 3,
        1, 6, 2,
        1, 5, 6,
        1, 4, 5,
        1, 0, 4,
        0, 3, 7,
        0, 7, 4,
        2, 6, 7,
        2, 7, 3,
        5, 4, 7,
        5, 7, 6
    };

    GLuint vertexArrayObject;
    glGenVertexArrays(1, &vertexArrayObject);
    glBindVertexArray(vertexArrayObject);

    GLuint vertexBufferObject;
    glGenBuffers(1, &vertexBufferObject);
    glBindBuffer(GL_ARRAY_BUFFER, vertexBufferObject);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    GLuint elementBufferObject;
    glGenBuffers(1, &elementBufferObject);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, elementBufferObject);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), nullptr);
    glEnableVertexAttribArray(0);
    glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, nullptr);

    glDeleteVertexArrays(1, &vertexArrayObject);
    glDeleteBuffers(1, &vertexBufferObject);
    glDeleteBuffers(1, &elementBufferObject);
  }
};

#endif // SPACESHOOTER_BOX_H
