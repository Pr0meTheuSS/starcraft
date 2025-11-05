#pragma once
#include <cmath>

#include "formation.hpp"

namespace rts::ai {

class GridFormation final : public IFormationStrategy {
   public:
    explicit GridFormation(float spacing = 40.0f) : spacing(spacing) {}

    std::unordered_map<int, Vector2> computeTargets(const std::vector<Entity*>& selected, const Vector2& target) const override {
        std::unordered_map<int, Vector2> result;
        const int count = static_cast<int>(selected.size());
        const int cols = static_cast<int>(std::ceil(std::sqrt(static_cast<float>(count))));
        const int rows = (count + cols - 1) / cols;

        int i = 0;
        for (int r = 0; r < rows; ++r) {
            for (int c = 0; c < cols && i < count; ++c, ++i) {
                const Vector2 offset{(c - cols / 2.0f) * spacing, (r - rows / 2.0f) * spacing};
                result[selected[i]->id()] = {target.x + offset.x, target.y + offset.y};
            }
        }
        return result;
    }

   private:
    float spacing;
};

}  // namespace rts::ai
