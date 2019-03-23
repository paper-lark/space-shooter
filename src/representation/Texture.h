#ifndef SPACESHOOTER_TEXTURE_H
#define SPACESHOOTER_TEXTURE_H

#include <glad/glad.h>
#include <string>

class Texture {
    GLuint ID;        // texture object ID
    std::string mapType; // map type TODO: refactor into type enum
    std::string path;
    GLuint textureType; // texture type

    void bindImage(GLenum target, const std::string &path);

public:
    Texture(const std::string &type, const std::string &path);
    Texture(const std::string &type, const std::vector<std::string> &path);
    void bind(GLenum texture = GL_TEXTURE0) const;
    const std::string &getName() const {
        return mapType;
    }

    GLuint getID() const {
        return ID;
    }

    const std::string &getPath() const {
        return path;
    }
};


#endif //SPACESHOOTER_TEXTURE_H
