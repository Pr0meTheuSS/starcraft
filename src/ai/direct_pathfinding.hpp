#pragma once
#include <cmath>

#include "pathfinding.hpp"

namespace rts::ai {

class DirectPathfinding final : public IPathfindingStrategy {
   public:
    Vector2 nextStep(const Vector2& current, const Vector2& target, float speed, float dt) const override {
        Vector2 delta{target.x - current.x, target.y - current.y};
        float dist = std::sqrt(delta.x * delta.x + delta.y * delta.y);
        if (dist < 1.0f) return target;

        Vector2 dir{delta.x / dist, delta.y / dist};
        float step = std::min(dist, speed * dt);
        return {current.x + dir.x * step, current.y + dir.y * step};
    }
};

}  // namespace rts::ai
