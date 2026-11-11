#pragma once
#include <cstdint>
#include "config.hpp"

enum class CursorType : uint8_t {
    OS_default,
    none,
};

class Cursor {
    CursorType type = CursorType::OS_default;
public:
    Cursor() = default;
    ~Cursor() = default;
    void setType(const CursorType type);
    CursorType getType() const { return type; }
private:
    t1_disable_copy_and_move(Cursor)
};
