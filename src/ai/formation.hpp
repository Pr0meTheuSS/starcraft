#pragma once

#include <unordered_map>
#include <vector>

#include "raylib.h"

namespace rts::ai {
struct IFormationStrategy {
    virtual ~IFormationStrategy() = default;
    virtual std::unordered_map<int, Vector2> computeTargets(const std::vector<Entity*>& selected, const Vector2& target) const = 0;
};

}  // namespace rts::ai

