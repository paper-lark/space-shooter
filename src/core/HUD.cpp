#include "HUD.h"
#include "Callback.h"

HUD::HUD(const std::string &path) {
  SPDLOG_INFO("Creating HUD instance");
  FT_Library library;
  FT_Face font;

  // load library
  if (FT_Init_FreeType(&library)) {
    throw std::runtime_error("Failed to initialize FreeType library");
  }
  if (FT_New_Face(library, path.c_str(), 0, &font)) {
    throw std::runtime_error("Failed to load font: " + path);
  }
  FT_Set_Pixel_Sizes(font, 0, 128);

  // generate glyph bitmaps for first 128 ASCII characters
  SPDLOG_INFO("Rendering character bitmaps...");
  glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
  for (GLubyte ch = 0; ch < 128; ch++) {
    // render character
    if (FT_Load_Char(font, ch, unsigned(FT_LOAD_RENDER))) {
      throw std::runtime_error("Failed to load glyph");
    }

    // load texture
    GLuint texture;
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, font->glyph->bitmap.width, font->glyph->bitmap.rows, 0, GL_RED,
                 GL_UNSIGNED_BYTE, font->glyph->bitmap.buffer);
    // set texture options
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    // store character
    Character character = {texture, glm::ivec2(font->glyph->bitmap.width, font->glyph->bitmap.rows),
                           glm::ivec2(font->glyph->bitmap_left, font->glyph->bitmap_top),
                           GLuint(font->glyph->advance.x)};
    characters.insert(std::make_pair(GLchar(ch), character));
  } // TODO: reuse `Texture` class
  SPDLOG_INFO("Current map size: {}", characters.size());

  // release library resources
  FT_Done_Face(font);
  FT_Done_FreeType(library);

  // create vertex array buffer
  glGenVertexArrays(1, &vao);
  glGenBuffers(1, &vbo);
  glBindVertexArray(vao);
  glBindBuffer(GL_ARRAY_BUFFER, vbo);
  glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * 6 * 4, nullptr, GL_DYNAMIC_DRAW);
  glEnableVertexAttribArray(0);
  glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), nullptr);
  glBindBuffer(GL_ARRAY_BUFFER, 0);
  glBindVertexArray(0);
}

float HUD::CalculateTextWidth(const std::string &text, GLfloat scale) const {
  float result = 0.f;
  for (GLchar c : text) {
    const Character &ch = characters.at(c); // TODO: what if character is not present?
    result += (ch.advance >> 6u) * scale;
  }

  return result;
}

void HUD::RenderText(Shader &shader, const std::string &text, glm::vec2 position, GLfloat scale) const {
  // iterate through the text
  for (GLchar c : text) {
    const Character &ch = characters.at(c); // TODO: what if character is not present?
    GLfloat xPos = position.x + ch.bearing.x * scale;
    GLfloat yPos = position.y - (ch.size.y - ch.bearing.y) * scale;
    GLfloat w = ch.size.x * scale;
    GLfloat h = ch.size.y * scale;

    // update VBO for each character
    GLfloat vertices[6][4] = {{xPos, yPos + h, 0.0, 0.0}, {xPos, yPos, 0.0, 1.0},     {xPos + w, yPos, 1.0, 1.0},
                              {xPos, yPos + h, 0.0, 0.0}, {xPos + w, yPos, 1.0, 1.0}, {xPos + w, yPos + h, 1.0, 0.0}};

    // update content of VBO memory
    glBindVertexArray(vao);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    // render quad
    glBindVertexArray(vao);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, ch.textureID);
    glDrawArrays(GL_TRIANGLES, 0, 6);
    position.x += (ch.advance >> 6u) * scale;
  }

  // bind dummy vao and texture
  glBindVertexArray(0);
  glBindTexture(GL_TEXTURE_2D, 0);
}

void HUD::Draw(Shader &shader, glm::ivec2 windowSize, unsigned health, unsigned score) {
  // setup shader
  shader.use();
  glm::mat4 projectionMatrix = glm::ortho(0.f, float(windowSize.x), 0.f, float(windowSize.y));
  shader.setMatrix("projection", projectionMatrix);
  shader.setVec3("textColor", glm::vec3(0.9f, 0.9f, 0.9f));

  // draw statistics
  RenderText(shader, "Health: " + std::to_string(health), glm::vec2(10.f, 10.f), 0.25f);
  RenderText(shader, "Score:  " + std::to_string(score), glm::vec2(10.f, 34.f), 0.25f);

  if (health == 0) {
    // draw game over
    float width = CalculateTextWidth("Game over", 0.75f);
    RenderText(shader, "Game over", glm::vec2(windowSize.x * 0.5f - width * 0.5f, windowSize.y * 0.5f - 2.0f), 0.75f);
  } else {
    // draw crosshairs
    // TODO: find something better
    float width = CalculateTextWidth("+", 0.25f);
    RenderText(shader, "+", glm::vec2(windowSize.x * 0.5f - width * 0.5, windowSize.y * 0.5f - 2.0f), 0.25f);
  }
}

HUD::~HUD() {
  SPDLOG_INFO("Destructing HUD");
  glDeleteVertexArrays(1, &vao);
  glDeleteBuffers(1, &vbo);
}