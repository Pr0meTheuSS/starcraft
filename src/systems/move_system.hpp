#pragma once
#include <memory>
#include <unordered_map>
#include <vector>

#include "../ai/avoidance.hpp"
#include "../ai/formation.hpp"
#include "../ai/pathfinding.hpp"
#include "../components/available_to_move.hpp"
#include "raylib.h"
#include "system.hpp"

namespace rts::systems {

using namespace rts::ai;

class MoveSystem : public System {
   public:
    MoveSystem(EntityRegistry& reg, Camera2D& cam, float speed, std::shared_ptr<IFormationStrategy> formation, std::shared_ptr<IAvoidanceStrategy> avoidance,
               std::shared_ptr<IPathfindingStrategy> pathfinding)
        : System(reg),
          camera(cam),
          moveSpeed(speed),
          formationStrategy(std::move(formation)),
          avoidanceStrategy(std::move(avoidance)),
          pathfindingStrategy(std::move(pathfinding)) {}

    void update(float dt) override {
        processCommands();   // принимаем новые цели
        updateMovement(dt);  // двигаем всё остальное
    }

   private:
    Camera2D& camera;
    float moveSpeed;
    std::unordered_map<int, Vector2> activeTargets;  // id -> цель

    std::shared_ptr<IFormationStrategy> formationStrategy;
    std::shared_ptr<IAvoidanceStrategy> avoidanceStrategy;
    std::shared_ptr<IPathfindingStrategy> pathfindingStrategy;

    // --- 1. Обработка команд ---
    void processCommands() {
        if (!IsMouseButtonPressed(MOUSE_RIGHT_BUTTON)) return;
        if (!formationStrategy) return;

        const Vector2 clickTarget = GetScreenToWorld2D(GetMousePosition(), camera);

        std::vector<Entity*> selected;
        for (auto e : registry.find_all_with<rts::components::Position, rts::components::Selectable, rts::components::AvailableToMove>()) {
            const auto sel = e->get<rts::components::Selectable>();
            if (sel->selected) selected.push_back(e);
        }

        if (selected.empty()) return;

        // Каждому выбранному задаём новую цель (перезаписываем старую)
        const auto newTargets = formationStrategy->computeTargets(selected, clickTarget);
        for (const auto& [id, tgt] : newTargets) {
            activeTargets[id] = tgt;
        }
    }

    // --- 2. Обновление движения ---
    void updateMovement(float dt) {
        if (!pathfindingStrategy || !avoidanceStrategy) return;

        std::vector<Entity*> allEntities;
        for (auto e : registry.find_all_with<rts::components::Position>()) {
            allEntities.push_back(e);
        }

        std::vector<int> reached;  // кто дошёл

        for (auto e : registry.find_all_with<rts::components::Position>()) {
            const int id = e->id();
            if (!activeTargets.contains(id)) continue;  // нет активной цели

            auto pos = e->get<rts::components::Position>();
            const Vector2& target = activeTargets[id];

            // Шаг навигации
            const Vector2 next = pathfindingStrategy->nextStep(pos->pos, target, moveSpeed, dt);
            pos->pos = next;

            // Отталкивание
            const float minDist = 25.0f;
            const Vector2 repulsion = avoidanceStrategy->computeRepulsion(e, allEntities, minDist);
            pos->pos.x += repulsion.x;
            pos->pos.y += repulsion.y;

            // Проверка достижения цели
            const float dx = target.x - pos->pos.x;
            const float dy = target.y - pos->pos.y;
            if (dx * dx + dy * dy < 4.0f) {  // 2px радиус
                reached.push_back(id);
            }
        }

        // Удаляем завершивших движение
        for (int id : reached) {
            activeTargets.erase(id);
        }
    }
};

}  // namespace rts::systems
