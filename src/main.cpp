#include <cmath>
#include <cstring>
#include <vector>
//
#include <SDL3/SDL.h>
#include "engine/render/glad/glad.h"
//
#include "engine/debug/logger.hpp"
#include "engine/io/folders.hpp"
#include "engine/window/window.hpp"
#include "engine/window/input/controls.hpp"
#include "engine/window/input/bindnames.hpp"
#include "engine/render/renderer.hpp"
#include "engine/render/camera.hpp"
#include "engine/render/material_atlas.hpp"
#include "engine/render/mesh_pool.hpp"
#include "engine/render/texture_rect.hpp"
#include "engine/render/draw_item.hpp"
//
#include <glm/vec3.hpp>
#include <glm/mat4x4.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/geometric.hpp>

static debug::Logger logger("demo");

int main() {
    debug::Logger::init("aod_demo_log.txt");
    Controls::initDefaults();

    try {
        MainWindow window("AoD Demo - 3D Renderer", PixelCoord(1280, 720));
        window.setFPS(60);

        // GL-функции должны быть загружены через GLAD сразу после создания контекста.
        // gladLoadGL принимает функцию-загрузчик; SDL_GL_GetProcAddress возвращает её.
        if (!gladLoadGL()) {
            logger.error("Failed to load GL functions via GLAD.");
            return 1;
        }
        logger.info("OpenGL version: {}",
                    reinterpret_cast<const char*>(glGetString(GL_VERSION)));

        Renderer renderer;
        Camera camera;
        camera.position = glm::vec3(0.0f, 4.0f, 8.0f);
        camera.yaw = 0.0f;       // взгляд в -Z
        camera.pitch = -0.25f;
        camera.fov = 60.0f;
        camera.aspect = float(window.getSize().x) / float(window.getSize().y);

        MaterialAtlas atlas;
        atlas.buildProcedural();
        const TextureRect whiteRect  = atlas.rect(0);
        const TextureRect checkerRect = atlas.rect(1);

        MeshPool meshPool;
        const MeshId cubeMesh  = meshPool.createCube(1.0f);
        const MeshId planeMesh = meshPool.createPlane(20.0f);

        renderer.init(meshPool, atlas);
        renderer.setViewport(0, 0, int(window.getSize().x), int(window.getSize().y));

        glm::vec3 cubePos(0.0f, 0.5f, 0.0f);
        bool lookActive = false;
        constexpr float moveSpeed = 5.0f; // units/sec

        while (window.isOpen()) {
            window.pollEvents();
            Input& input = window.getInput();

            if (input.jactive(close_action))
                window.close();

            // Переключение free-look по RMB
            const bool lookNow = input.active(look_active);
            if (lookNow != lookActive) {
                lookActive = lookNow;
                window.setRelativeMouse(lookActive);
            }

            // dt
            float dt = float(window.getRealFrameDelayMs()) / 1000.0f;
            if (dt <= 0.0f || dt > 0.1f) dt = 0.016f;

            // Horizontal forward (без pitch) и right
            const float sy = std::sin(camera.yaw);
            const float cy = std::cos(camera.yaw);
            const glm::vec3 hforward(-sy, 0.0f, -cy);
            const glm::vec3 right(cy, 0.0f, -sy);
            const glm::vec3 up(0.0f, 1.0f, 0.0f);

            glm::vec3 move(0.0f);
            if (input.active(move_forward)) move += hforward;
            if (input.active(move_back))    move -= hforward;
            if (input.active(move_right))   move += right;
            if (input.active(move_left))    move -= right;
            if (input.active(move_up))      move += up;
            if (input.active(move_down))    move -= up;
            if (float len = glm::length(move); len > 0.0001f) {
                cubePos += (move / len) * moveSpeed * dt;
            }

            // Free-look
            if (look_active) {
                const PixelCoord delta = input.getMouseDelta();
                camera.yaw   += float(delta.x) * 0.003f;
                camera.pitch -= float(delta.y) * 0.003f;
                if (camera.pitch >  1.5f) camera.pitch =  1.5f;
                if (camera.pitch < -1.5f) camera.pitch = -1.5f;
            }

            if (window.justResized()) {
                camera.aspect = float(window.getSize().x) / float(window.getSize().y);
                renderer.setViewport(0, 0, int(window.getSize().x), int(window.getSize().y));
            }

            // Очередь отрисовки
            std::vector<DrawItem> items;
            items.reserve(2);

            // Plane
            DrawItem plane{};
            plane.mesh   = planeMesh;
            plane.uvRect = checkerRect;
            const glm::mat4 planeModel(1.0f);
            std::memcpy(plane.transform, glm::value_ptr(planeModel), sizeof(plane.transform));
            plane.tint = 0xFFFFFFFFu;
            plane.flags = 0;
            items.push_back(plane);

            // Cube
            DrawItem cube{};
            cube.mesh    = cubeMesh;
            cube.uvRect = whiteRect;
            const glm::mat4 cubeModel = glm::translate(glm::mat4(1.0f), cubePos);
            std::memcpy(cube.transform, glm::value_ptr(cubeModel), sizeof(cube.transform));
            cube.tint = 0xFFFFFFFFu;
            cube.flags = 0;
            items.push_back(cube);

            renderer.beginFrame(camera);
            renderer.submit(items);
            renderer.endFrame();
            window.present();
        }
        logger.info("Demo exited cleanly.");
    }
    catch (const std::exception& exception) {
        logger.error("Exception: {}", exception.what());
        return 1;
    }
    return 0;
}
