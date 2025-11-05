#pragma once
#include <cmath>

#include "avoidance.hpp"
#include "../components/position.hpp"

namespace rts::ai {

class SimpleAvoidance final : public IAvoidanceStrategy {
   public:
    explicit SimpleAvoidance(float strength = 0.05f) : strength(strength) {}

    Vector2 computeRepulsion(Entity* self, const std::vector<Entity*>& neighbors, float minDist) const override {
        Vector2 total{0, 0};
        const auto pos1 = self->get<rts::components::Position>();

        for (auto other : neighbors) {
            if (self == other) continue;
            const auto pos2 = other->get<rts::components::Position>();
            const Vector2 diff{pos1->pos.x - pos2->pos.x, pos1->pos.y - pos2->pos.y};
            const float dist2 = diff.x * diff.x + diff.y * diff.y;

            if (dist2 < minDist * minDist && dist2 > 0.001f) {
                const float factor = (minDist * minDist - dist2) / (minDist * minDist);
                total.x += diff.x * factor;
                total.y += diff.y * factor;
            }
        }

        total.x *= strength;
        total.y *= strength;
        return total;
    }

   private:
    float strength;
};

}  // namespace rts::ai
