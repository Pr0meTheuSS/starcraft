#pragma once
#include "component.hpp"
#include "raylib.h"

namespace rts::components {

struct Renderable : Component {
    Color color;
    float radius;
    Renderable(Color c, float r) : color(c), radius(r) {}
};
}  // namespace rts::components