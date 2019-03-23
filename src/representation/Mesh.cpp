#include "Mesh.h"
#include <spdlog/spdlog.h>
#include <iostream>

void Mesh::setupMesh() {
    // create buffers and array
    glGenVertexArrays(1, &vao);
    glGenBuffers(1, &vbo);
    glGenBuffers(1, &ebo);
    glBindVertexArray(vao);

    // load VBO data
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), vertices.data(), GL_STATIC_DRAW);

    // load EBO data
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned), indices.data(), GL_STATIC_DRAW);

    // map attributes
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, position));
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, normal));
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, textureCoords));

    // bind array
    glBindVertexArray(0);
    SPDLOG_DEBUG("Created mesh with {} vertices ({})", vertices.size(), vao);
}

void Mesh::Draw(Shader shader) const {

    // bind textures
    unsigned currentDiffuse = 1;
    unsigned currentSpecular = 1;
    unsigned currentEmission = 1;
    unsigned current = 0;
    for (const Texture &texture : textures) {
        std::string number;
        const std::string &name = texture.getName();
        texture.bind(GL_TEXTURE0 + current);

        if (name == "texture_diffuse") {
            number = std::to_string(currentDiffuse++);
        } else if (name == "texture_specular") {
            number = std::to_string(currentSpecular++);
        } else if (name == "texture_emission") {
            number = std::to_string(currentEmission++);
        } else {
            throw std::runtime_error("Unknown texture type: " + name);
        }

        std::string uniformLabel = std::string("material.").append(name).append(number);
        shader.setFloat(uniformLabel, current);
        current++;
    }

    // draw object
    glBindVertexArray(vao);
    glDrawElements(GL_TRIANGLES, GLsizei(indices.size()), GL_UNSIGNED_INT, nullptr);
    glBindVertexArray(0);
}
