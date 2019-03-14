#ifndef MAIN_SHADER_H
#define MAIN_SHADER_H

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <string>

class Shader {
    // shader program ID
    GLuint ID;

public:
    // constructor
    Shader(const std::string &vertexPath, const std::string &fragmentPath);

    // use shader program
    void use();

    // set uniform variables
    void setBool(const std::string &name, bool value) const;
    void setInt(const std::string &name, int value) const;
    void setFloat(const std::string &name, float value) const;
    void setMatrix(const std::string &name, glm::mat4 matrix) const;
};


#endif //MAIN_SHADER_H