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
  FT_Set_Pixel_Sizes(font, 0, 64);

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
    glTexImage2D(
        GL_TEXTURE_2D,
        0,
        GL_RED,
        font->glyph->bitmap.width,
        font->glyph->bitmap.rows,
        0,
        GL_RED,
        GL_UNSIGNED_BYTE,
        font->glyph->bitmap.buffer
    );
    // set texture options
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    // store character
    Character character = {
        texture,
        glm::ivec2(font->glyph->bitmap.width, font->glyph->bitmap.rows),
        glm::ivec2(font->glyph->bitmap_left, font->glyph->bitmap_top),
        GLuint(font->glyph->advance.x)
    };
    characters.insert(std::make_pair(GLchar(ch), character));
  }
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

void HUD::RenderText(Shader &shader, const std::string &text, GLfloat x, GLfloat y, GLfloat scale, glm::vec3 color) {

  // activate shader
  shader.setVec3("textColor", color);

  // iterate through the text
  for (GLchar c : text) {
    Character ch = characters[c]; // TODO: what if character is not present?
    GLfloat xPos = x + ch.bearing.x * scale;
    GLfloat yPos = y - (ch.size.y - ch.bearing.y) * scale;
    GLfloat w = ch.size.x * scale;
    GLfloat h = ch.size.y * scale;

    // update VBO for each character
    GLfloat vertices[6][4] = {
        { xPos,     yPos + h,   0.0, 0.0 },
        { xPos,     yPos,       0.0, 1.0 },
        { xPos + w, yPos,       1.0, 1.0 },
        { xPos,     yPos + h,   0.0, 0.0 },
        { xPos + w, yPos,       1.0, 1.0 },
        { xPos + w, yPos + h,   1.0, 0.0 }
    };

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
    x += (ch.advance >> 6u) * scale;
  }

  // bind dummy vao and texture
  glBindVertexArray(0);
  glBindTexture(GL_TEXTURE_2D, 0);
}

void HUD::Draw(Shader &shader) {
  shader.use();
  glm::mat4 projectionMatrix = glm::ortho(0.f, 800.f, 0.f, 600.f); // TODO: not working
  shader.setMatrix("projection", projectionMatrix);
  RenderText(shader, "Some text", 10.f, 10.f, 1.0f, glm::vec3(0.5f, 0.8f, 0.2f));
}