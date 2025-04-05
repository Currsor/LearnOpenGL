#pragma once
#include <glad/glad.h>
#include <string>
#include <iostream>

class Texture {
public:
    Texture(const std::string& path, bool flipVertically = true, GLenum target = GL_TEXTURE_2D);
    ~Texture();

    void Bind(GLenum textureUnit = GL_TEXTURE0) const;
    void Unbind() const;

    void SetWrap(GLenum axis, GLint mode);
    void SetFilter(GLenum filterType, GLint mode);

    inline int GetWidth() const { return width; }
    inline int GetHeight() const { return height; }
    inline GLuint GetID() const { return id; }

private:
    GLuint id;
    int width;
    int height;
    int channels;
    GLenum target;
};