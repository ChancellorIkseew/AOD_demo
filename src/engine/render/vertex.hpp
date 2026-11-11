#pragma once
#include <cstdint>
#include "config.hpp"

/// Минимальный вертекс 3D-рендерера: 24 байта.
/// - pos[3]:   позиция в пространстве модели (12 байт)
/// - uv[2]:    локальные UV в [0,1] (не атласные!) (8 байт)
/// - color[4]: RGBA ubyte, нормализуется GPU в [0,1] (тинт/повреждения) (4 байта)
struct Vertex {
    float   pos[3];
    float   uv[2];
    uint8_t color[4];
};
static_assert(sizeof(Vertex) == 24, "Vertex must be 24 bytes (12+8+4, alignment 4)");
