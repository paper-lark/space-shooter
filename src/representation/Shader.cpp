#include "Shader.h"
#include <fstream>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>
#include <spdlog/spdlog.h>

// Load shader specified by its source file and type
GLuint loadShader(GLenum type, const std::string &filename) {
  // read shader from file
  std::ifstream fs(filename);
  if (!fs.is_open()) {
    throw std::runtime_error("Failed to load shader file: " + filename);
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
    throw std::runtime_error(std::string("Shader compilation failed").append(infoLog));
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
    throw std::runtime_error(std::string("Shader linking failed: ").append(infoLog));
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

void Shader::setUint(const std::string &name, unsigned value) const {
  glUniform1ui(glGetUniformLocation(ID, name.c_str()), value);
}

void Shader::setMatrix(const std::string &name, glm::mat4 matrix) const {
  glUniformMatrix4fv(glGetUniformLocation(ID, name.c_str()), 1, GL_FALSE, glm::value_ptr(matrix));
}

void Shader::setVec4(const std::string &name, glm::vec4 vector) const {
  glUniform4fv(glGetUniformLocation(ID, name.c_str()), 1, glm::value_ptr(vector));
}

void Shader::setVec3(const std::string &name, glm::vec3 vector) const {
  glUniform3fv(glGetUniformLocation(ID, name.c_str()), 1, glm::value_ptr(vector));
}