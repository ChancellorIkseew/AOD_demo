#pragma once
#include <cstddef>
#include "config.hpp"
#include "engine/render/vertex.hpp"

class Mesh {
    unsigned int m_vao = 0, m_vbo = 0, m_ibo = 0;
    int          m_indexCount = 0;
    unsigned int m_indexType  = 0;  // GL_UNSIGNED_INT
public:
    Mesh();
    ~Mesh();
    Mesh(Mesh&& other) noexcept;
    Mesh& operator=(Mesh&& other) noexcept;
    //
    void upload(const Vertex* verts, size_t vcount, const uint32_t* indices, size_t icount);
    void bind() const;
    void draw() const;
    bool valid() const { return m_vao != 0; }
private:
    t1_disable_copy(Mesh)
};
