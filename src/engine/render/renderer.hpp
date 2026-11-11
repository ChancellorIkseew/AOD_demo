#pragma once
#include <span>
#include "config.hpp"
#include "engine/render/draw_item.hpp"
#include "engine/render/shader_program.hpp"

class MainWindow;
class Camera;
class MeshPool;
class MaterialAtlas;

class Renderer {
    ShaderProgram* m_shader = nullptr;
    MeshPool*      m_meshPool = nullptr;
    MaterialAtlas* m_atlas     = nullptr;
public:
    Renderer();
    ~Renderer();
    //
    /// Загрузить шейдер (вызывает после создания GL-контекста и загрузки GLAD)
    void init(MeshPool& meshPool, MaterialAtlas& atlas);
    //
    void beginFrame(const Camera& cam);
    void submit(std::span<DrawItem> items);
    void endFrame();   // (swap делает MainWindow::present)
    //
    void setViewport(int x, int y, int w, int h);
private:
    t1_disable_copy_and_move(Renderer)
};
