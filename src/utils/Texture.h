//
// Created by Max Zhuravsky on 2019-02-19.
//

#ifndef MAIN_TEXTURE_H
#define MAIN_TEXTURE_H

#include <glad/glad.h>
#include <string>

class Texture {
    // texture object ID
    GLuint ID;

public:
    Texture(const std::string &filename);
    void bind(GLenum texture = GL_TEXTURE0) const;
};


#endif //MAIN_TEXTURE_H
