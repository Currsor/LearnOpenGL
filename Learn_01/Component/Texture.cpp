#include "Texture.h"
#include "../stb_image.h"

Texture::Texture(const std::string& path, bool flipVertically, GLenum target) 
    : id(0), width(0), height(0), channels(0), target(target) {
    glGenTextures(1, &id);
    glBindTexture(target, id);

    // 默认纹理参数
    SetWrap(GL_TEXTURE_WRAP_S, GL_REPEAT);
    SetWrap(GL_TEXTURE_WRAP_T, GL_REPEAT);
    SetFilter(GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    SetFilter(GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    stbi_set_flip_vertically_on_load(flipVertically);
    unsigned char* data = stbi_load(path.c_str(), &width, &height, &channels, 0);
    if (data) {
        GLenum format = GL_RGB;
        if (channels == 1)
            format = GL_RED;
        else if (channels == 4)
            format = GL_RGBA;

        glTexImage2D(target, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(target);
    } else {
        std::cerr << "Failed to load texture: " << path << std::endl;
    }
    stbi_image_free(data);
}

Texture::~Texture() {
    glDeleteTextures(1, &id);
}

void Texture::Bind(GLenum textureUnit) const {
    glActiveTexture(textureUnit);
    glBindTexture(target, id);
}

void Texture::Unbind() const {
    glBindTexture(target, 0);
}

void Texture::SetWrap(GLenum axis, GLint mode) {
    glBindTexture(target, id);
    glTexParameteri(target, axis, mode);
}

void Texture::SetFilter(GLenum filterType, GLint mode) {
    glBindTexture(target, id);
    glTexParameteri(target, filterType, mode);
}