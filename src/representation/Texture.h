#ifndef SPACESHOOTER_TEXTURE_H
#define SPACESHOOTER_TEXTURE_H

#include <glad/glad.h>
#include <string>
#include "TextureMapType.h"

class Texture {
    GLuint ID;        // texture object ID
    TextureMapType mapType; // map type
    std::string path; // path to texture, used to check if the texture has been already loaded
    GLuint textureType; // texture type

    void bindImage(GLenum target, const std::string &path);

public:
    Texture(TextureMapType type, const std::string &path);
    Texture(TextureMapType type, const std::vector<std::string> &path);
    void bind(GLenum texture = GL_TEXTURE0) const;
    TextureMapType getName() const {
        return mapType;
    }

    const std::string &getPath() const {
        return path;
    }
};


#endif //SPACESHOOTER_TEXTURE_H
