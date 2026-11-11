#include "renderer.hpp"
//
#include <algorithm>
#include <cstring>
#include "glad/glad.h"
#include "engine/debug/logger.hpp"
#include "engine/io/folders.hpp"
#include "engine/render/camera.hpp"
#include "engine/render/mesh.hpp"
#include "engine/render/mesh_pool.hpp"
#include "engine/render/material_atlas.hpp"

static debug::Logger logger("renderer");

Renderer::Renderer() = default;

Renderer::~Renderer() {
    delete m_shader;
}

void Renderer::init(MeshPool& meshPool, MaterialAtlas& atlas) {
    m_meshPool = &meshPool;
    m_atlas    = &atlas;

    m_shader = new ShaderProgram(
        io::folders::SHADERS / "static.vert",
        io::folders::SHADERS / "static.frag"
    );

    // OpenGL default state
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);
    // GL_CULL_FACE намеренно отключён на старте — упрощает демо (не надо думать о winding).
    // Включить после верификации порядка вершин всех мешей.
    glDisable(GL_CULL_FACE);
    glDisable(GL_BLEND);
    glEnable(GL_MULTISAMPLE);

    logger.info("Renderer initialized.");
}

void Renderer::setViewport(int x, int y, int w, int h) {
    glViewport(x, y, w, h);
}

void Renderer::beginFrame(const Camera& cam) {
    glClearColor(0.12f, 0.14f, 0.18f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    m_shader->use();
    m_atlas->bind(0);
    m_shader->setUniformInt("u_atlas", 0);

    glm::mat4 vp = cam.viewProjection();
    m_shader->setUniformMat4("u_vp", &vp[0][0]);
}

void Renderer::submit(std::span<DrawItem> items) {
    if (!m_shader || !m_meshPool) return;
    // Сортировка: opaque (flags==0) сначала. Внутри — стабильный порядок.
    std::stable_partition(items.begin(), items.end(),
        [](const DrawItem& di){ return (di.flags & 0x1) == 0; });

    for (const DrawItem& di : items) {
        // uv_rect — это (x_offset, y_offset, w_scale, h_scale)
        const float uv_rect[4] = { di.uvRect.x, di.uvRect.y, di.uvRect.w, di.uvRect.h };
        m_shader->setUniformVec4("u_uvRect", uv_rect);
        m_shader->setUniformMat4("u_model", di.transform);

        // tint как vec4 [0,1] из uint32_t RGBA
        const float tint_vec[4] = {
            float((di.tint >> 24) & 0xFF) / 255.0f,
            float((di.tint >> 16) & 0xFF) / 255.0f,
            float((di.tint >>  8) & 0xFF) / 255.0f,
            float((di.tint      ) & 0xFF) / 255.0f,
        };
        m_shader->setUniformVec4("u_tint", tint_vec);

        const Mesh& mesh = m_meshPool->get(di.mesh);
        if (mesh.valid()) {
            mesh.bind();
            mesh.draw();
        }
    }
}

void Renderer::endFrame() {
    // No-op здесь; swap делает MainWindow::present().
}
