#pragma once

#include "raylib.h"

namespace rts::ai {

struct IPathfindingStrategy {
    virtual ~IPathfindingStrategy() = default;
    virtual Vector2 nextStep(const Vector2& current, const Vector2& target, float speed, float dt) const = 0;
};

}  // namespace rts::ai
