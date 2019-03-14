#include "Shader.h"
#include <fstream>
#include <iostream>
#include <glm/gtc/type_ptr.hpp>

// Load shader specified by its source file and type
GLuint loadShader(GLenum type, const std::string &filename) {
    // read shader from file
    std::ifstream fs(filename);
    if (!fs.is_open()) {
        std::cerr << "Failed to load shader file: " << filename << std::endl;
        throw std::runtime_error("Shader missing");
    }
    std::string shaderText((std::istreambuf_iterator<char>(fs)), std::istreambuf_iterator<char>());

    // compile shader
    GLuint shaderObject = glCreateShader(type); // create shader object
    const GLchar *source = shaderText.c_str();
    glShaderSource(shaderObject, 1, &source, nullptr);
    glCompileShader(shaderObject);

    // check for compilation errors
    GLint compilationStatus;
    glGetShaderiv(shaderObject, GL_COMPILE_STATUS, &compilationStatus);

    if (compilationStatus != GL_TRUE) {
        GLchar infoLog[512];
        glGetShaderInfoLog(shaderObject, 512, nullptr, infoLog);
        std::cerr << "Shader compilation failed: " << std::endl << infoLog << std::endl;
        throw std::runtime_error("Shader compilation failed");
    }

    return shaderObject;
}


Shader::Shader(const std::string &vertexPath, const std::string &fragmentPath) {
    GLuint vertexShader = loadShader(GL_VERTEX_SHADER, vertexPath);
    GLuint fragmentShader = loadShader(GL_FRAGMENT_SHADER, fragmentPath);

    // create shader program
    this->ID = glCreateProgram();
    glAttachShader(this->ID, vertexShader);
    glAttachShader(this->ID, fragmentShader);
    glLinkProgram(this->ID);
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    // check for errors
    GLint success;
    glGetProgramiv(this->ID, GL_LINK_STATUS, &success);
    if (success != GL_TRUE) {
        GLchar infoLog[512];
        glGetProgramInfoLog(this->ID, 512, nullptr, infoLog);
        std::cerr << "Shader linking failed: " << std::endl << infoLog << std::endl;
        throw std::runtime_error("Shader linking failed");
    }
}

void Shader::use() {
    glUseProgram(this->ID);
}

void Shader::setBool(const std::string &name, bool value) const {
    glUniform1i(glGetUniformLocation(ID, name.c_str()), (int)value);
}

void Shader::setFloat(const std::string &name, float value) const {
    glUniform1f(glGetUniformLocation(ID, name.c_str()), value);
}

void Shader::setInt(const std::string &name, int value) const {
    glUniform1i(glGetUniformLocation(ID, name.c_str()), value);
}

void Shader::setMatrix(const std::string &name, glm::mat4 matrix) const {
    // TODO: is matrix copied?
    glUniformMatrix4fv(glGetUniformLocation(ID, name.c_str()), 1, GL_FALSE, glm::value_ptr(matrix));
}