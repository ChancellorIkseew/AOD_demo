#include "cursor.hpp"
//
#include <SDL3/SDL.h>

void Cursor::setType(const CursorType t) {
    type = t;
    if (t == CursorType::none)
        SDL_HideCursor();
    else
        SDL_ShowCursor();
}
