#pragma once
#include <optional>
#include <vector>
#include "config.hpp"
#include "engine/render/texture_2d.hpp"
#include "engine/render/texture_rect.hpp"

/// Атлас материалов: сетка N×N тайлов размером TILE_SIZE.
/// tile_index → TextureRect (offset.xy, scale.wh в [0,1] UV-пространстве атласа).
class MaterialAtlas {
    static constexpr int TILE_SIZE      = 64;
    static constexpr int TILES_PER_ROW  = 16;
    static constexpr int ATLAS_SIZE     = TILE_SIZE * TILES_PER_ROW; // 1024
    std::optional<Texture2D> m_texture;
    std::vector<TextureRect> m_rects;
public:
    MaterialAtlas() = default;
    //
    /// Создать процедурный атлас с двумя тайлами: 0=white, 1=checker.
    void buildProcedural();
    //
    TextureRect rect(int tileIndex) const;
    void bind(unsigned int unit) const;
    int tileCount() const { return static_cast<int>(m_rects.size()); }
private:
    t1_disable_copy_and_move(MaterialAtlas)
};
