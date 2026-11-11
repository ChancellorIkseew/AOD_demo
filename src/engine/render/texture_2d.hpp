#pragma once
#include <cstdint>
#include "config.hpp"

enum class TextureFilter : uint8_t {
    nearest,
    linear,
    linear_mipmap,
};

class Texture2D {
    unsigned int m_textureID = 0;
public:
    Texture2D(int width, int height, const unsigned char* data,
              TextureFilter filter = TextureFilter::nearest);
    ~Texture2D();
    //
    unsigned int getID() const { return m_textureID; }
private:
    t1_disable_copy_and_move(Texture2D)
};
