#include "controls.hpp"
//
#include <SDL3/SDL_scancode.h>
#include <SDL3/SDL_mouse.h>
#include "bindnames.hpp"
#include "engine/debug/logger.hpp"

std::unordered_map<std::string, Binding> Controls::bindings;
static debug::Logger logger("controls");

void Controls::addBinding(const std::string& bindName, int code, InputType type) {
    bindings.insert_or_assign(bindName, Binding(code, type));
}

void Controls::initDefaults() {
    addBinding(move_forward, SDL_SCANCODE_W,      InputType::keyboard);
    addBinding(move_back,    SDL_SCANCODE_S,      InputType::keyboard);
    addBinding(move_left,    SDL_SCANCODE_A,      InputType::keyboard);
    addBinding(move_right,   SDL_SCANCODE_D,      InputType::keyboard);
    addBinding(move_up,      SDL_SCANCODE_SPACE,  InputType::keyboard);
    addBinding(move_down,    SDL_SCANCODE_LSHIFT, InputType::keyboard);
    addBinding(look_active,  SDL_BUTTON_RIGHT,    InputType::mouse);
    addBinding(close_action, SDL_SCANCODE_ESCAPE, InputType::keyboard);
    logger.info("Default bindings initialized ({} bindings).", bindings.size());
}
