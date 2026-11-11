#pragma once
#include <optional>
#include <string_view>
#include "binding.hpp"
#include "engine/coords/pixel_coord.hpp"

struct SDL_Window;
union SDL_Event;
class MainWindow;

class Input {
    std::optional<Binding> lastKeyPressed;
    std::string_view textEntered;
    PixelCoord mouseCoord;
    PixelCoord mouseDelta;
    MouseWheelScroll mouseWheelScroll = MouseWheelScroll::none;
    bool textInputActive = false;
public:
    Input() = default;

    bool active(cString bindName) const;
    bool jactive(cString bindName) const;
    bool released(cString bindName) const;
    bool jactiveOrRepeated(cString bindName) const;

    PixelCoord getMouseCoord() const { return mouseCoord; }
    PixelCoord getMouseDelta() const { return mouseDelta; }
    MouseWheelScroll getMouseWheelScroll() const { return mouseWheelScroll; }

    std::optional<Binding> getLastKeyPressed() const { return lastKeyPressed; }
    std::string_view getTextEntered() const { return textEntered; }
    void enableTextInput(const bool flag) { textInputActive = flag; }

    void consumeMouseClicks();
private:
    friend MainWindow;
    void update(const SDL_Event& event);
    void reset(SDL_Window* sdlWindow);
    t1_disable_copy_and_move(Input)
};
