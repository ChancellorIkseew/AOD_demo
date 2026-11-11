#include "camera.hpp"
//
#include <cmath>
#include <glm/gtc/matrix_transform.hpp>

glm::vec3 Camera::forward() const {
    // yaw=0, pitch=0 → (0, 0, -1) — стандартный взгляд OpenGL в -Z
    return glm::vec3(
        -std::sin(yaw) * std::cos(pitch),
         std::sin(pitch),
        -std::cos(yaw) * std::cos(pitch)
    );
}

glm::mat4 Camera::viewMatrix() const {
    return glm::lookAt(position, position + forward(), glm::vec3(0, 1, 0));
}

glm::mat4 Camera::projectionMatrix() const {
    return glm::perspective(glm::radians(fov), aspect, nearP, farP);
}

glm::mat4 Camera::viewProjection() const {
    return projectionMatrix() * viewMatrix();
}
