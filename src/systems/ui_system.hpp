#pragma once
#include "raylib.h"
#define RAYGUI_IMPLEMENTATION
#include <functional>
#include <string>
#include <unordered_map>
#include <vector>

#include "raygui.h"

namespace rts::systems {

struct ResourceInfo {
    int gold = 500;
    int wood = 300;
    int stone = 200;
    int population = 15;
};

struct UnitInfo {
    std::string name;
    int health;
    Texture2D icon;
};

class UiSystem {
   public:
    UiSystem() {
        GuiLoadStyleDefault();
        GuiSetStyle(DEFAULT, TEXT_SIZE, 18);

        buildOptions = {"House", "Barracks", "Farm", "Watchtower"};
    }

    void setResources(const ResourceInfo& r) { resources = r; }
    void setSelectedUnits(const std::vector<UnitInfo>& units) { selectedUnits = units; }

    void update(float dt) { DrawPanels(); }

    // Колбэк при постройке
    std::function<void(const std::string& building)> onBuildRequest;

   private:
    ResourceInfo resources;
    std::vector<UnitInfo> selectedUnits;
    std::vector<std::string> buildOptions;
    int selectedBuildIndex = 0;

    void DrawPanels() {
        const int screenW = GetScreenWidth();
        const int screenH = GetScreenHeight();

        // === Верхняя панель ресурсов ===
        DrawRectangle(0, 0, screenW, 40, Color{25, 25, 25, 220});
        DrawText(TextFormat("Gold: %d", resources.gold), 20, 10, 18, GOLD);
        DrawText(TextFormat("Wood: %d", resources.wood), 180, 10, 18, BROWN);
        DrawText(TextFormat("Stone: %d", resources.stone), 340, 10, 18, GRAY);
        DrawText(TextFormat("Pop: %d", resources.population), 500, 10, 18, RAYWHITE);

        // === Правая панель юнитов ===
        const int rightPanelW = 240;
        const int rightPanelX = screenW - rightPanelW;
        DrawRectangle(rightPanelX, 40, rightPanelW, screenH - 120, Color{30, 30, 30, 240});
        DrawText("Selected Units", rightPanelX + 10, 50, 20, RAYWHITE);

        int y = 80;
        for (const auto& unit : selectedUnits) {
            if (unit.icon.id > 0) DrawTexture(unit.icon, rightPanelX + 10, y, WHITE);

            DrawText(unit.name.c_str(), rightPanelX + 70, y + 10, 18, RAYWHITE);
            DrawRectangle(rightPanelX + 70, y + 30, 100, 8, DARKGRAY);
            DrawRectangle(rightPanelX + 70, y + 30, unit.health, 8, RED);
            y += 60;
        }

        // === Нижняя панель ===
        const int bottomH = 80;
        const int bottomY = screenH - bottomH;
        DrawRectangle(0, bottomY, screenW, bottomH, Color{25, 25, 25, 220});

        DrawText("Build:", 20, bottomY + 25, 20, RAYWHITE);

        // Выпадашка выбора здания
        Rectangle dropRect{100, (float)bottomY + 20, 200, 35};
        GuiSetStyle(DROPDOWNBOX, TEXT_ALIGNMENT, TEXT_ALIGN_LEFT);

        static bool dropEdit = false;
        GuiDropdownBox(dropRect, buildOptionsStr().c_str(), &selectedBuildIndex, dropEdit);
        if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON) && CheckCollisionPointRec(GetMousePosition(), dropRect)) {
            dropEdit = !dropEdit;
        }

        // Кнопка "Построить"
        if (GuiButton(Rectangle{320, (float)bottomY + 20, 120, 35}, "Build")) {
            if (onBuildRequest) onBuildRequest(buildOptions[selectedBuildIndex]);
        }
    }

    std::string buildOptionsStr() const {
        std::string s;
        for (size_t i = 0; i < buildOptions.size(); ++i) {
            s += buildOptions[i];
            if (i + 1 < buildOptions.size()) s += ";";
        }
        return s;
    }
};

}  // namespace rts::systems
