#pragma once
#include "config.hpp"
#include <glm/vec3.hpp>
#include <glm/mat4x4.hpp>

class Camera {
public:
    glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f);
    float yaw   = 0.0f;   // вокруг Y, радианы. yaw=0 → взгляд в -Z
    float pitch = 0.0f;   // вокруг X, радианы. Ограничено ±1.5
    float fov   = 60.0f;  // градусы
    float aspect = 16.0f / 9.0f;
    float nearP = 0.1f;
    float farP  = 1000.0f;
    //
    glm::vec3 forward() const;
    glm::mat4 viewMatrix() const;
    glm::mat4 projectionMatrix() const;
    glm::mat4 viewProjection() const;
};
