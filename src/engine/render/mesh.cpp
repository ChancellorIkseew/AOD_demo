#include "mesh.hpp"
//
#include "glad/glad.h"
#include "engine/debug/logger.hpp"

static debug::Logger logger("mesh");

Mesh::Mesh() = default;

Mesh::~Mesh() {
    if (m_vao) glDeleteVertexArrays(1, &m_vao);
    if (m_vbo) glDeleteBuffers(1, &m_vbo);
    if (m_ibo) glDeleteBuffers(1, &m_ibo);
}

Mesh::Mesh(Mesh&& o) noexcept {
    m_vao = o.m_vao; m_vbo = o.m_vbo; m_ibo = o.m_ibo;
    m_indexCount = o.m_indexCount; m_indexType = o.m_indexType;
    o.m_vao = o.m_vbo = o.m_ibo = 0;
}

Mesh& Mesh::operator=(Mesh&& o) noexcept {
    if (this != &o) {
        this->~Mesh();
        m_vao = o.m_vao; m_vbo = o.m_vbo; m_ibo = o.m_ibo;
        m_indexCount = o.m_indexCount; m_indexType = o.m_indexType;
        o.m_vao = o.m_vbo = o.m_ibo = 0;
    }
    return *this;
}

void Mesh::upload(const Vertex* verts, size_t vcount, const uint32_t* indices, size_t icount) {
    glCreateVertexArrays(1, &m_vao);
    glCreateBuffers(1, &m_vbo);
    glCreateBuffers(1, &m_ibo);

    glNamedBufferStorage(m_vbo, vcount * sizeof(Vertex), verts, 0);
    glNamedBufferStorage(m_ibo, icount * sizeof(uint32_t), indices, 0);

    // Привязка атрибутов через DSA
    glVertexArrayVertexBuffer(m_vao, 0, m_vbo, 0, sizeof(Vertex));
    glVertexArrayElementBuffer(m_vao, m_ibo);

    glEnableVertexArrayAttrib(m_vao, 0);  // pos
    glEnableVertexArrayAttrib(m_vao, 1);  // uv
    glEnableVertexArrayAttrib(m_vao, 2);  // color

    glVertexArrayAttribFormat(m_vao, 0, 3, GL_FLOAT, GL_FALSE, offsetof(Vertex, pos));
    glVertexArrayAttribFormat(m_vao, 1, 2, GL_FLOAT, GL_FALSE, offsetof(Vertex, uv));
    glVertexArrayAttribFormat(m_vao, 2, 4, GL_UNSIGNED_BYTE, GL_TRUE, offsetof(Vertex, color));

    glVertexArrayAttribBinding(m_vao, 0, 0);
    glVertexArrayAttribBinding(m_vao, 1, 0);
    glVertexArrayAttribBinding(m_vao, 2, 0);

    m_indexCount = static_cast<int>(icount);
    m_indexType  = GL_UNSIGNED_INT;
    logger.debug("Mesh uploaded: {} verts, {} indices.", vcount, icount);
}

void Mesh::bind() const {
    glBindVertexArray(m_vao);
}

void Mesh::draw() const {
    glDrawElements(GL_TRIANGLES, m_indexCount, m_indexType, nullptr);
}
