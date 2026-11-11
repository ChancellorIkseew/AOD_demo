#include "window.hpp"
//
#include <SDL3/SDL.h>
#include <algorithm>
#include <stdexcept>
#include "engine/debug/logger.hpp"

static debug::Logger logger("main_window");

MainWindow::MainWindow(const std::string& title, const PixelCoord size) : size(size) {
    if (!SDL_Init(SDL_INIT_VIDEO))
        throw std::runtime_error("Could not init SDL");

    // OpenGL 4.6 Core Profile
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 6);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
    SDL_GL_SetAttribute(SDL_GL_RED_SIZE,   8);
    SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, 8);
    SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE,  8);
    SDL_GL_SetAttribute(SDL_GL_ALPHA_SIZE, 8);
    SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
    SDL_GL_SetAttribute(SDL_GL_MULTISAMPLEBUFFERS, 1);
    SDL_GL_SetAttribute(SDL_GL_MULTISAMPLESAMPLES, 4);

    sdlWindow = SDL_CreateWindow(title.c_str(), int(size.x), int(size.y),
                                 SDL_WINDOW_RESIZABLE | SDL_WINDOW_OPENGL);
    if (!sdlWindow) {
        const std::string error = SDL_GetError();
        SDL_Quit();
        throw std::runtime_error("SDL_CreateWindow Error: " + error);
    }
    glContext = SDL_GL_CreateContext(sdlWindow);
    if (!glContext) {
        const std::string error = SDL_GetError();
        SDL_DestroyWindow(sdlWindow);
        SDL_Quit();
        throw std::runtime_error("SDL_GL_CreateContext Error: " + error);
    }
    if (!SDL_GL_MakeCurrent(sdlWindow, static_cast<SDL_GLContext>(glContext)))
        logger.error("SDL_GL_MakeCurrent failed: {}", SDL_GetError());
    if (!SDL_GL_SetSwapInterval(1))
        logger.warning("VSync not available");
    logger.info("Window created {}x{}, OpenGL 4.6 Core context ready.", int(size.x), int(size.y));
}

MainWindow::~MainWindow() {
    if (glContext) SDL_GL_DestroyContext(static_cast<SDL_GLContext>(glContext));
    if (sdlWindow) SDL_DestroyWindow(sdlWindow);
    SDL_Quit();
}

void MainWindow::setFPS(const uint64_t newFps) {
    fps = newFps;
    requiredDelayNs = NS_PER_SECOND / fps;
}

void MainWindow::setRelativeMouse(const bool enabled) {
    relativeMouse = enabled;
    SDL_SetWindowRelativeMouseMode(sdlWindow, enabled);
    if (enabled) SDL_HideCursor();
    else         SDL_ShowCursor();
}

void MainWindow::pollEvents() {
    input.reset(sdlWindow);
    SDL_Event event;
    resized = lostFocus = gainedFocus = false;
    while (SDL_PollEvent(&event)) {
        switch (event.type) {
        case SDL_EVENT_QUIT:
            close();
            break;
        case SDL_EVENT_WINDOW_RESIZED: {
            resized = true;
            int x = 0, y = 0;
            SDL_GetWindowSize(sdlWindow, &x, &y);
            size = PixelCoord(x, y);
            break;
        }
        case SDL_EVENT_WINDOW_FOCUS_LOST:
            lostFocus = true; focused = false; break;
        case SDL_EVENT_WINDOW_FOCUS_GAINED:
            gainedFocus = true; focused = true; break;
        default:
            input.update(event);
            break;
        }
    }
    lostFocus = lostFocus && !focused;
    gainedFocus = gainedFocus && focused;
}

void MainWindow::makeDelay() {
    const uint64_t frameTimeNs = SDL_GetTicksNS() - frameStartNs;
    if (frameTimeNs < requiredDelayNs)
        SDL_DelayNS(requiredDelayNs - frameTimeNs);
    realDelayNs = std::max(frameTimeNs, requiredDelayNs);
    frameStartNs += realDelayNs;
}

void MainWindow::present() {
    SDL_GL_SwapWindow(sdlWindow);
    makeDelay();
}
