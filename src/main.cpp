#include <algorithm>
#include <cmath>
#include <iostream>
#include <map>
#include <memory>
#include <optional>
#include <typeindex>
#include <vector>

#include "ai/circle_formation.hpp"
#include "ai/direct_pathfinding.hpp"
#include "ai/simple_avoidance.hpp"
#include "components/position.hpp"
#include "components/renderable.hpp"
#include "components/selectable.hpp"
#include "raylib.h"
#include "systems/move_system.hpp"
#include "systems/render_system.hpp"
#include "systems/selection_system.hpp"
#include "systems/ui_system.hpp"

int main() {
    InitWindow(1280, 720, "RTS ECS Prototype (2D)");
    SetTargetFPS(60);

    Camera2D camera = {0};
    camera.target = {0, 0};
    camera.offset = {640, 360};
    camera.zoom = 1.0f;

    rts::EntityRegistry registry;
    rts::systems::UiSystem ui;
    // создаём юниты
    for (int i = 0; i < 3; ++i) {
        auto& e = registry.create();
        e.add<rts::components::Position>(Vector2{static_cast<float>(100 + 100 * i), static_cast<float>(150 + 50 * i)});
        e.add<rts::components::Selectable>();
        e.add<rts::components::Renderable>(rts::components::Renderable{RED, 15.0f});
    }

    // системы
    rts::systems::RenderSystem renderSys(registry, camera);
    rts::systems::SelectionSystem selectSys(registry, camera);

    std::shared_ptr<rts::ai::IPathfindingStrategy> pathfindingStrategy = std::make_shared<rts::ai::DirectPathfinding>();
    std::shared_ptr<rts::ai::IAvoidanceStrategy> avoidanceStrategy = std::make_shared<rts::ai::SimpleAvoidance>();
    std::shared_ptr<rts::ai::IFormationStrategy> formationStrategy = std::make_shared<rts::ai::CircleFormation>();

    rts::systems::MoveSystem moveSys(registry, camera, 50, formationStrategy, avoidanceStrategy, pathfindingStrategy);

    while (!WindowShouldClose()) {
        // управление камерой TODO: move to controller
        if (IsKeyDown(KEY_RIGHT)) camera.target.x += 5 / camera.zoom;
        if (IsKeyDown(KEY_LEFT)) camera.target.x -= 5 / camera.zoom;
        if (IsKeyDown(KEY_DOWN)) camera.target.y += 5 / camera.zoom;
        if (IsKeyDown(KEY_UP)) camera.target.y -= 5 / camera.zoom;
        camera.zoom += GetMouseWheelMove() * 0.1f;
        camera.zoom = std::clamp(camera.zoom, 0.5f, 2.5f);

        BeginDrawing();
        ClearBackground(DARKGREEN);

        // порядок систем
        moveSys.update(GetFrameTime());
        renderSys.update(GetFrameTime());
        selectSys.update(GetFrameTime());
        ui.update(GetFrameTime());

        EndDrawing();
    }

    CloseWindow();
    return 0;
}
