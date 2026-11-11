#pragma once
#include <compare>
#include "config.hpp"

struct PixelCoord {
    float x = 0.0f, y = 0.0f;

    constexpr PixelCoord(const int x, const int y) noexcept : x(static_cast<float>(x)), y(static_cast<float>(y)) {}
    constexpr PixelCoord(const float x, const float y) noexcept : x(x), y(y) {}
    constexpr PixelCoord() noexcept = default;

    t1_finline_cxpr auto operator<=>(const PixelCoord& rhs) const noexcept = default;

    t1_finline_cxpr PixelCoord operator+(const PixelCoord rhs) const noexcept {
        return { x + rhs.x, y + rhs.y };
    }
    t1_finline_cxpr PixelCoord operator-(const PixelCoord rhs) const noexcept {
        return { x - rhs.x, y - rhs.y };
    }
    t1_finline_cxpr PixelCoord operator*(const float value) const noexcept {
        return { x * value, y * value };
    }
};
