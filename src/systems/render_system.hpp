#include "raylib.h"
#include "system.hpp"

namespace rts::systems {
class RenderSystem : public System {
   public:
    RenderSystem(EntityRegistry& reg, Camera2D& cam) : System(reg), camera(cam) {}

    void update(float dt) override {
        BeginMode2D(camera);

        // TODO: move to components
        for (int x = -1000; x < 2000; x += 16) {
            for (int y = -1000; y < 2000; y += 16) {
                DrawRectangleLines(x, y, 64, 64, Color{0, 80, 0, 100});
            }
        }

        for (auto e : registry.find_all_with<rts::components::Position, rts::components::Renderable, rts::components::Selectable>()) {
            auto pos = e->get<rts::components::Position>();
            auto rend = e->get<rts::components::Renderable>();
            auto sel = e->get<rts::components::Selectable>();

            DrawCircleV(pos->pos, rend->radius, sel->selected ? YELLOW : rend->color);
            if (sel->selected) DrawCircleLines(pos->pos.x, pos->pos.y, rend->radius + 3, GOLD);
        }

        EndMode2D();
    }

   private:
    Camera2D& camera;
};

}  // namespace rts::systems
