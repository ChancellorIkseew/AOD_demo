#pragma once
#include <cstdint>
#include "engine/render/texture_rect.hpp"

using MeshId = uint32_t;

/// Элемент очереди отрисовки. Заполняется игровой логикой за кадр.
/// Renderer сортирует по flags (opaque сначала), внутри — порядок сохраняется.
struct DrawItem {
    MeshId      mesh;          ///< индекс в MeshPool
    TextureRect uvRect;       ///< прямоугольник в атласе (offset.xy, scale.wh)
    float       transform[16]; ///< mat4 column-major (model matrix)
    uint32_t    tint;          ///< RGBA (дополнительный per-draw тинт)
    uint8_t     flags;         ///< bit0: transparent
    uint8_t     pad[3];
};
