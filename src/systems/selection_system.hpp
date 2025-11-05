#pragma once

#include "../components/position.hpp"
#include "../components/selectable.hpp"
// TODO: remove dependency on raylib
#include "raylib.h"
#include "system.hpp"

namespace rts::systems {
class SelectionSystem : public System {
   public:
    SelectionSystem(EntityRegistry& reg, Camera2D& cam) : System(reg), camera(cam) {}

    void update(float dt) override {
        if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
            selectStart = GetScreenToWorld2D(GetMousePosition(), camera);
            selecting = true;
        }

        if (IsMouseButtonReleased(MOUSE_LEFT_BUTTON)) {
            Vector2 end = GetScreenToWorld2D(GetMousePosition(), camera);
            Rectangle selRect = {fminf(selectStart.x, end.x), fminf(selectStart.y, end.y), fabsf(end.x - selectStart.x), fabsf(end.y - selectStart.y)};

            for (auto e : registry.find_all_with<rts::components::Position, rts::components::Selectable>()) {
                auto pos = e->get<rts::components::Position>();
                auto sel = e->get<rts::components::Selectable>();
                sel->selected = CheckCollisionPointRec(pos->pos, selRect);
            }
            selecting = false;
        }

        // визуал выделения
        if (selecting) {
            BeginMode2D(camera);
            Vector2 cur = GetScreenToWorld2D(GetMousePosition(), camera);
            DrawRectangleLines(fminf(selectStart.x, cur.x), fminf(selectStart.y, cur.y), fabsf(cur.x - selectStart.x), fabsf(cur.y - selectStart.y), SKYBLUE);
            EndMode2D();
        }
    }

   private:
    Camera2D& camera;
    bool selecting = false;
    Vector2 selectStart{};
};

}  // namespace rts::systems
