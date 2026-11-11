#pragma once
#include <cstdint>
#include <string>
#include "config.hpp"
#include "cursor.hpp"
#include "engine/coords/pixel_coord.hpp"
#include "engine/window/input/input.hpp"

struct SDL_Window;

class MainWindow {
    static constexpr uint64_t NS_PER_SECOND = 1'000'000'000, NS_PER_MS = 1'000'000;
    PixelCoord size;
    Cursor cursor;
    Input input;
    SDL_Window* sdlWindow = nullptr;
    void*       glContext = nullptr;  // SDL_GLContext (opaque)
    uint64_t fps = 60, requiredDelayNs = NS_PER_SECOND / 60, realDelayNs = 0, frameStartNs = 0;
    bool open = true, resized = false, lostFocus = false, gainedFocus = false, focused = true;
    bool relativeMouse = false;
public:
    MainWindow(const std::string& title, const PixelCoord size);
    ~MainWindow();
    //
    void close() { open = false; }
    void setFPS(const uint64_t fps);
    void setRelativeMouse(const bool enabled);
    void present();   // swap buffers + frame delay
    //
    bool isOpen()        const noexcept { return open; }
    bool justResized()   const noexcept { return resized; }
    bool hasLostFocus()  const noexcept { return lostFocus; }
    bool hasGainedFocus()const noexcept { return gainedFocus; }
    bool isFocused()     const noexcept { return focused; }
    uint64_t getFPS()    const noexcept { return fps; }
    uint64_t getRealFrameDelayNs() const noexcept { return realDelayNs; }
    uint64_t getRealFrameDelayMs() const noexcept { return realDelayNs / NS_PER_MS; }
    PixelCoord getSize() const noexcept { return size; }
    //
    SDL_Window* getSDLWindow() const noexcept { return sdlWindow; }
    void*       getGLContext() const noexcept { return glContext; }
    //
    Cursor& getCursor() { return cursor; }
    Input&  getInput()  { return input; }
    //
    void pollEvents();
private:
    void makeDelay();
    t1_disable_copy_and_move(MainWindow)
};
