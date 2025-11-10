#pragma once

#include <vector>

#include "entities/entity.hpp"

namespace rts {

class EntityRegistry {
   public:
    Entity& create() {
        entities_.push_back(std::make_unique<Entity>(nextId_++));
        return *entities_.back();
    }

    template <typename... Cs>
    std::vector<Entity*> find_all_with() {
        std::vector<Entity*> result;
        for (auto& e : entities_) {
            if ((e->has<Cs>() && ...)) result.push_back(e.get());
        }
        return result;
    }

   private:
    std::vector<std::unique_ptr<Entity>> entities_;
    EntityID nextId_ = 1;
};
}  // namespace rts
