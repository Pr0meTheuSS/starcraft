#pragma once

#include <cstddef>
#include <map>
#include <memory>
#include <typeindex>

#include "../components/component.hpp"

namespace rts {

using EntityID = std::size_t;

class Entity {
   public:
    Entity(EntityID id) : id_(id) {}

    template <typename T, typename... Args>
    T& add(Args&&... args) {
        auto comp = std::make_shared<T>(std::forward<Args>(args)...);
        components_[std::type_index(typeid(T))] = comp;
        return *comp;
    }

    template <typename T>
    std::shared_ptr<T> get() {
        auto it = components_.find(std::type_index(typeid(T)));
        if (it != components_.end()) return std::static_pointer_cast<T>(it->second);
        return nullptr;
    }

    template <typename T>
    bool has() const {
        return components_.count(std::type_index(typeid(T))) > 0;
    }

    EntityID id() const { return id_; }

   private:
    EntityID id_;
    std::map<std::type_index, std::shared_ptr<rts::components::Component>> components_;
};
}  // namespace rts
