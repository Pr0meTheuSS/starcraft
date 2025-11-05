#pragma once

#include <vector>

#include "../entities/entity.hpp"
#include "raylib.h"

namespace rts::ai {
struct IAvoidanceStrategy {
    virtual ~IAvoidanceStrategy() = default;
    virtual Vector2 computeRepulsion(Entity* self, const std::vector<Entity*>& neighbors, float minDist) const = 0;
};

}  // namespace rts::ai
