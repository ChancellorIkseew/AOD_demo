#include "material_atlas.hpp"
//
#include <cstring>
#include "glad/glad.h"
#include "engine/debug/logger.hpp"

static debug::Logger logger("material_atlas");

void MaterialAtlas::buildProcedural() {
    // Буфер атласа: 1024×1024 RGBA8, инициализирован белым.
    std::vector<unsigned char> buf(ATLAS_SIZE * ATLAS_SIZE * 4, 255);

    // Тайл 0: white — уже заполнен белым (255,255,255,255).
    // Сохраним TextureRect для тайла 0.
    const float tileSizeNormalized = static_cast<float>(TILE_SIZE) / static_cast<float>(ATLAS_SIZE);
    m_rects.push_back(TextureRect{ 0.0f, 0.0f, tileSizeNormalized, tileSizeNormalized }); // tile 0 at (0,0)

    // Тайл 1: checker 8×8 внутри 64×64 (каждая клетка 8×8 пикселей).
    const int tileX = 1 * TILE_SIZE;
    const int tileY = 0;
    for (int y = 0; y < TILE_SIZE; ++y) {
        for (int x = 0; x < TILE_SIZE; ++x) {
            const int cx = x / 8;
            const int cy = y / 8;
            const bool white = ((cx + cy) & 1) == 0;
            const unsigned char c = white ? 255 : 64;
            const int idx = ((tileY + y) * ATLAS_SIZE + (tileX + x)) * 4;
            buf[idx + 0] = c;
            buf[idx + 1] = c;
            buf[idx + 2] = c;
            buf[idx + 3] = 255;
        }
    }
    m_rects.push_back(TextureRect{ tileSizeNormalized * 1.0f, 0.0f, tileSizeNormalized, tileSizeNormalized }); // tile 1 at (ts, 0)

    m_texture.emplace(ATLAS_SIZE, ATLAS_SIZE, buf.data(), TextureFilter::linear_mipmap);

    // DSA: wrap = REPEAT (для будущих тайловых поверхностей; для демо не обязательно, но не вредит)
    glTextureParameteri(m_texture->getID(), GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTextureParameteri(m_texture->getID(), GL_TEXTURE_WRAP_T, GL_REPEAT);

    logger.info("Procedural atlas built: {}x{}, {} tiles.", ATLAS_SIZE, ATLAS_SIZE, m_rects.size());
}

TextureRect MaterialAtlas::rect(int tileIndex) const {
    if (tileIndex < 0 || tileIndex >= int(m_rects.size())) {
        logger.error("Tile index out of range: {}", tileIndex);
        return NULL_TEXTURE_RECT;
    }
    return m_rects[tileIndex];
}

void MaterialAtlas::bind(unsigned int unit) const {
    if (m_texture)
        glBindTextureUnit(unit, m_texture->getID());
}
