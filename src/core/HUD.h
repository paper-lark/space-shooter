#ifndef SPACESHOOTER_HUD_H
#define SPACESHOOTER_HUD_H

#include "../representation/Shader.h"
#include <ft2build.h>
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>
#include <map>
#include <spdlog/spdlog.h>
#include FT_FREETYPE_H

struct Character {
  GLuint textureID;   // ID handle of the glyph texture
  glm::ivec2 size;    // Size of glyph
  glm::ivec2 bearing; // Offset from baseline to left/top of glyph
  GLuint advance;     // Offset to advance to next glyph
};

class HUD {
  std::map<GLchar, Character> characters;
  GLuint vao;
  GLuint vbo;

  void RenderText(Shader &shader, const std::string &text, glm::vec2 position, GLfloat scale);

public:
  HUD(const std::string &path);
  HUD(const HUD &) = delete;
  ~HUD();

  // Draw HUD on the screen
  void Draw(Shader &shader, glm::ivec2 windowSize, unsigned health, unsigned score);
};

#endif // SPACESHOOTER_HUD_H
