#include "surface.hpp"
//
#include <SDL3/SDL_surface.h>

Surface::~Surface() noexcept {
    SDL_DestroySurface(m_surface);
}
Surface::Surface(Surface&& other) noexcept {
    m_surface = other.m_surface;
    other.m_surface = nullptr;
}
Surface& Surface::operator=(Surface&& other) noexcept {
    if (this != &other) {
        SDL_DestroySurface(m_surface);
        m_surface = other.m_surface;
        other.m_surface = nullptr;
    }
    return *this;
}
