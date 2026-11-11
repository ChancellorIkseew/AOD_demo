#pragma once
#include <vector>
#include "engine/render/mesh.hpp"

using MeshId = uint32_t;

class MeshPool {
    std::vector<Mesh> m_meshes;
public:
    MeshPool() = default;
    //
    /// Загрузить готовый массив вершин/индексов
    MeshId upload(const Vertex* verts, size_t vcount, const uint32_t* indices, size_t icount);
    /// Куб размера size (от -size/2 до +size/2)
    MeshId createCube(float size);
    /// Плоскость size × size на Y=0
    MeshId createPlane(float size);
    //
    const Mesh& get(MeshId id) const;
    size_t count() const { return m_meshes.size(); }
};
