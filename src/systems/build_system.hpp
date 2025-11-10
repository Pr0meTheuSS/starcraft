#pragma once

#include <cmath>

#include "components/build_preview.hpp"
#include "components/position.hpp"
#include "components/renderable.hpp"
#include "raylib.h"
#include "system.hpp"

namespace rts::systems {

class BuildSystem : public System {
   public:
    BuildSystem(EntityRegistry& reg, Camera2D& cam) : System(reg), camera(cam) {}

    void startPlacing(const std::string& buildingType, float size) {
        std::cout << buildingType << ::std::endl;
        // Создаем entity-превью
        preview = &registry.create();
        preview->add<rts::components::BuildPreview>();
        // preview->add<rts::components::Building>(buildingType, size);
        preview->add<rts::components::Position>(GetMouseWorld());
        preview->add<rts::components::Renderable>(BLUE, 20.0f);
        preview->add<rts::components::Selectable>();
    }

    void update(float dt) override {
        if (!preview) return;

        auto pos = preview->get<rts::components::Position>();
        pos->pos = GetMouseWorld();

        bool valid = !checkCollisions(*preview);
        preview->get<rts::components::BuildPreview>()->valid = valid;

        // Меняем цвет по валидности
        // auto render = preview->get<rts::components::Renderable>();
        // render->color = valid ? Fade(GREEN, 0.5f) : Fade(RED, 0.5f);

        // Завершить постройку
        if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON) && valid) {
            finalizeBuilding();
        }

        // Отменить
        if (IsMouseButtonPressed(MOUSE_RIGHT_BUTTON)) {
            cancelPreview();
        }
    }

   private:
    Camera2D& camera;
    Entity* preview = nullptr;

    Vector2 GetMouseWorld() { return GetScreenToWorld2D(GetMousePosition(), camera); }

    bool checkCollisions(Entity& e) {
        auto build = e.get<rts::components::BuildPreview>();
        auto pos = e.get<rts::components::Position>();

        for (auto other : registry.find_all_with<rts::components::BuildPreview, rts::components::Position>()) {
            if (&e == other) continue;

            auto oBuild = other->get<rts::components::BuildPreview>();
            auto oPos = other->get<rts::components::Position>();

            float dist = sqrt(oPos->pos.x * oPos->pos.x + oPos->pos.y * oPos->pos.y);
            if (dist < (build->getSize() + oBuild->getSize())) {
                return true;
            }
        }
        return false;
    }

    void finalizeBuilding() {
        auto render = preview->get<rts::components::Renderable>();
        // render->tint = WHITE;  // сделать обычным
        // preview->remove<rts::components::BuildPreview>();
        preview = nullptr;
    }

    void cancelPreview() {
        // registry.destroy_entity(*preview);
        preview = nullptr;
    }
};

}  // namespace rts::systems
