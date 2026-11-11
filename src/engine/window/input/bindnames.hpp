#pragma once
#include "binding.hpp"

// Действия демо
constexpr cString move_forward = "move_forward";
constexpr cString move_back    = "move_back";
constexpr cString move_left    = "move_left";
constexpr cString move_right   = "move_right";
constexpr cString move_up      = "move_up";
constexpr cString move_down    = "move_down";
constexpr cString look_active  = "look_active";
constexpr cString close_action = "close_action";

// Кнопки мыши (code = SDL_BUTTON_*)
constexpr cString LMB   = "LMB";
constexpr cString MidMB = "MidMB";
constexpr cString RMB   = "RMB";
