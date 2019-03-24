#include "Texture.h"
#define STB_IMAGE_IMPLEMENTATION
#include <iostream>
#include <spdlog/spdlog.h>
#include <stb_image.h>

void Texture::bindImage(GLenum target, const std::string &path) {
  int width, height, nrChannels;
  unsigned char *data = stbi_load(path.c_str(), &width, &height, &nrChannels, 0);

  if (data == nullptr) {
    throw std::runtime_error("Failed to load texture: " + path);
  }

  // decide format
  GLenum format = GL_RGB;
  if (nrChannels == 1) {
    format = GL_RED;
  } else if (nrChannels == 3) {
    format = GL_RGB;
  } else if (nrChannels == 4) {
    format = GL_RGBA;
  }
  SPDLOG_DEBUG("Data format resolved to {}", format == GL_RGB ? "RGB" : format == GL_RGBA ? "RGBA" : "RED");

  glTexImage2D(target, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
  stbi_image_free(data);
}

Texture::Texture(TextureMapType type, const std::vector<std::string> &paths)
    : mapType(type), path(paths[0]), textureType(GL_TEXTURE_CUBE_MAP) {
  // create texture object
  GLuint texture;
  glGenTextures(1, &texture);
  this->ID = texture;
  glBindTexture(GL_TEXTURE_CUBE_MAP, this->ID);

  // load texture faces
  for (GLuint i = 0; i < paths.size(); i++) {
    SPDLOG_INFO("Loading texture: {}", paths[i]);
    this->bindImage(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, paths[i]);
  }

  // specify filtering options
  glGenerateMipmap(GL_TEXTURE_CUBE_MAP);
  glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
}

Texture::Texture(TextureMapType type, const std::string &path) : mapType(type), path(path), textureType(GL_TEXTURE_2D) {
  // create texture object
  GLuint texture;
  glGenTextures(1, &texture);
  this->ID = texture;
  glBindTexture(GL_TEXTURE_2D, this->ID);

  // load texture image
  SPDLOG_INFO("Loading texture: {}", path);
  this->bindImage(GL_TEXTURE_2D, path);

  // set filtering options
  glGenerateMipmap(GL_TEXTURE_2D);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
}

void Texture::bind(GLenum texture) const {
  glActiveTexture(texture);
  glBindTexture(textureType, this->ID);
}