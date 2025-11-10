#pragma once

#include "ecs_core/entity_registry.hpp"

namespace rts::systems {
class System {
   public:
    explicit System(EntityRegistry& reg) : registry(reg) {}
    virtual void update(float dt) = 0;
    virtual ~System() = default;

   protected:
    EntityRegistry& registry;
};

}  // namespace rts::systems
