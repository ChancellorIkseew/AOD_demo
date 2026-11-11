#include "texture_2d.hpp"
//
#include <cmath>
#include <algorithm>
#include "glad/glad.h"

Texture2D::Texture2D(int width, int height, const unsigned char* data, TextureFilter filter) {
    glCreateTextures(GL_TEXTURE_2D, 1, &m_textureID);
    GLsizei levels = 1;
    if (filter == TextureFilter::linear_mipmap)
        levels = static_cast<GLsizei>(std::ceil(std::log2(static_cast<float>(std::max(width, height))))) + 1;
    glTextureStorage2D(m_textureID, levels, GL_RGBA8, width, height);
    glTextureSubImage2D(m_textureID, 0, 0, 0, width, height, GL_RGBA, GL_UNSIGNED_BYTE, data);
    if (filter == TextureFilter::linear_mipmap)
        glGenerateTextureMipmap(m_textureID);

    GLenum minF, magF;
    switch (filter) {
    case TextureFilter::nearest:      minF = GL_NEAREST;                magF = GL_NEAREST; break;
    case TextureFilter::linear:       minF = GL_LINEAR;                 magF = GL_LINEAR;  break;
    case TextureFilter::linear_mipmap: minF = GL_LINEAR_MIPMAP_LINEAR;   magF = GL_LINEAR;  break;
    default:                          minF = GL_NEAREST;                magF = GL_NEAREST; break;
    }
    glTextureParameteri(m_textureID, GL_TEXTURE_MIN_FILTER, minF);
    glTextureParameteri(m_textureID, GL_TEXTURE_MAG_FILTER, magF);
    glTextureParameteri(m_textureID, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTextureParameteri(m_textureID, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
}

Texture2D::~Texture2D() {
    glDeleteTextures(1, &m_textureID);
}
