#ifndef SPACESHOOTER_TEXTURE_H
#define SPACESHOOTER_TEXTURE_H

#include <glad/glad.h>
#include <string>

class Texture {
    GLuint ID;        // texture object ID
    std::string type; // texture type TODO: refactor into type enum
    std::string path;
public:
    Texture(const std::string &type, const std::string &path);
    void bind(GLenum texture = GL_TEXTURE0) const;
    const std::string &getName() const {
        return type;
    }

    GLuint getID() const {
        return ID;
    }

    const std::string &getPath() const {
        return path;
    }
};


#endif //SPACESHOOTER_TEXTURE_H
