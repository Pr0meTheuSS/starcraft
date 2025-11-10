#pragma once
#include <cmath>

#include "entities/entity.hpp"
#include "formation.hpp"

namespace rts::ai {

class CircleFormation final : public IFormationStrategy {
   public:
    explicit CircleFormation(float radius = 80.0f) : radius(radius) {}

    std::unordered_map<int, Vector2> computeTargets(const std::vector<Entity*>& selected, const Vector2& target) const override {
        std::unordered_map<int, Vector2> result;
        const int count = static_cast<int>(selected.size());
        const float step = 2 * PI / count;

        for (int i = 0; i < count; ++i) {
            const float angle = i * step;
            result[selected[i]->id()] = {target.x + std::cos(angle) * radius, target.y + std::sin(angle) * radius};
        }
        return result;
    }

   private:
    float radius;
};

}  // namespace rts::ai
