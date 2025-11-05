#pragma once
#include "component.hpp"
#include "raylib.h"

namespace rts::components {

struct Position : Component {
    Position(Vector2 pos) : pos(pos){};
    Vector2 pos;
};

}  // namespace rts::components