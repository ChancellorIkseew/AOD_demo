#include "mesh_pool.hpp"
//
#include <array>
#include "engine/debug/logger.hpp"

static debug::Logger logger("mesh_pool");

MeshId MeshPool::upload(const Vertex* verts, size_t vcount, const uint32_t* indices, size_t icount) {
    Mesh m;
    m.upload(verts, vcount, indices, icount);
    m_meshes.push_back(std::move(m));
    return static_cast<MeshId>(m_meshes.size() - 1);
}

MeshId MeshPool::createCube(float size) {
    const float h = size * 0.5f;
    // 6 граней × 4 вертекса = 24. UV [0,1]² на каждой грани.
    // Color = белый (255,255,255,255).
    const uint8_t W = 255;
    std::array<Vertex, 24> v;
    // +X
    v[ 0] = {{ h,-h,-h}, {0,0}, {W,W,W,W}};
    v[ 1] = {{ h,-h, h}, {1,0}, {W,W,W,W}};
    v[ 2] = {{ h, h, h}, {1,1}, {W,W,W,W}};
    v[ 3] = {{ h, h,-h}, {0,1}, {W,W,W,W}};
    // -X
    v[ 4] = {{-h,-h, h}, {0,0}, {W,W,W,W}};
    v[ 5] = {{-h,-h,-h}, {1,0}, {W,W,W,W}};
    v[ 6] = {{-h, h,-h}, {1,1}, {W,W,W,W}};
    v[ 7] = {{-h, h, h}, {0,1}, {W,W,W,W}};
    // +Y (top)
    v[ 8] = {{-h, h,-h}, {0,0}, {W,W,W,W}};
    v[ 9] = {{ h, h,-h}, {1,0}, {W,W,W,W}};
    v[10] = {{ h, h, h}, {1,1}, {W,W,W,W}};
    v[11] = {{-h, h, h}, {0,1}, {W,W,W,W}};
    // -Y (bottom)
    v[12] = {{-h,-h, h}, {0,0}, {W,W,W,W}};
    v[13] = {{ h,-h, h}, {1,0}, {W,W,W,W}};
    v[14] = {{ h,-h,-h}, {1,1}, {W,W,W,W}};
    v[15] = {{-h,-h,-h}, {0,1}, {W,W,W,W}};
    // +Z
    v[16] = {{ h,-h, h}, {0,0}, {W,W,W,W}};
    v[17] = {{-h,-h, h}, {1,0}, {W,W,W,W}};
    v[18] = {{-h, h, h}, {1,1}, {W,W,W,W}};
    v[19] = {{ h, h, h}, {0,1}, {W,W,W,W}};
    // -Z
    v[20] = {{-h,-h,-h}, {0,0}, {W,W,W,W}};
    v[21] = {{ h,-h,-h}, {1,0}, {W,W,W,W}};
    v[22] = {{ h, h,-h}, {1,1}, {W,W,W,W}};
    v[23] = {{-h, h,-h}, {0,1}, {W,W,W,W}};

    std::array<uint32_t, 36> idx = {
        0,1,2,  0,2,3,    // +X
        4,5,6,  4,6,7,    // -X
        8,9,10, 8,10,11,  // +Y
       12,13,14, 12,14,15,// -Y
       16,17,18, 16,18,19,// +Z
       20,21,22, 20,22,23 // -Z
    };
    MeshId id = upload(v.data(), v.size(), idx.data(), idx.size());
    logger.info("Cube created (size={}): meshId={}", size, id);
    return id;
}

MeshId MeshPool::createPlane(float size) {
    const float h = size * 0.5f;
    const uint8_t W = 255;
    // UV [0,1] — тайл атласа растягивается на всю плоскость.
    // Для чекер-тайла 8×8 это даёт 8×8 клеток видимых на плоскости.
    std::array<Vertex, 4> v = {{
        {{-h, 0, -h}, {0, 0}, {W,W,W,W}},
        {{ h, 0, -h}, {1, 0}, {W,W,W,W}},
        {{ h, 0,  h}, {1, 1}, {W,W,W,W}},
        {{-h, 0,  h}, {0, 1}, {W,W,W,W}},
    }};
    std::array<uint32_t, 6> idx = { 0, 1, 2,  0, 2, 3 };
    MeshId id = upload(v.data(), v.size(), idx.data(), idx.size());
    logger.info("Plane created (size={}): meshId={}", size, id);
    return id;
}

const Mesh& MeshPool::get(MeshId id) const {
    return m_meshes[id];
}
