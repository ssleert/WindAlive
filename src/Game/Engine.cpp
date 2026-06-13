module;
#include <imgui.h>
#include <memory>
#include <raylib.h>
#include <raymath.h>
#include <rlImGui.h>
#include <rlgl.h>
#include <stdint.h>
#include <vector>
#include <windalive.hpp>
export module Game.Engine;

import Game.Camera;
import Game.TexturesLoader;
import Game.Atlases.World;
import Game.Atlases.Tree;
import Game.Atlases.Rock;
import Game.WorldDrawer;
import Game.EntityDrawer;
import Game.World.Generator;
import Game.World.State;
import Game.ECS.State;

import Math.Vector;

namespace Game {
export class Engine
{
private:
  const Game::TexturesLoader texturesLoader;
  const Game::Atlases::World worldAtlas;
  const Game::Atlases::Tree treeAtlas;
  const Game::Atlases::Rock rockAtlas;

  Game::Camera camera;

  // TODO: rewrite to late initialization
  std::unique_ptr<Game::WorldDrawer> worldDrawer;
  std::unique_ptr<Game::EntityDrawer> entityDrawer;

  Game::World::State world;
  Game::ECS::State entities;

public:
  Engine()
    : entities(world)
  {
    auto generator = Game::World::Generator(1024, 1024);
    world = generator.generate();

    worldDrawer = std::make_unique<Game::WorldDrawer>(
      world, texturesLoader, worldAtlas, treeAtlas, rockAtlas, camera);

    entityDrawer = std::make_unique<Game::EntityDrawer>(entities, camera);

    rlImGuiSetup(true); // true is dark theme
  }

  ~Engine() { rlImGuiShutdown(); }

  fn tick() -> void
  {
    // TODO: move to server
    entities.tick();
  }

  fn input() -> void
  {
    ImGuiIO& io = ImGui::GetIO();
    bool imguiWantsInput = io.WantCaptureMouse || io.WantCaptureKeyboard ||
                           ImGui::IsAnyItemActive() ||
                           ImGui::IsAnyItemHovered();
    if (imguiWantsInput) {
      return;
    }
    entityDrawer->input();
    camera.input();
  }

  fn logic() -> void
  {
    camera.logic();
    entityDrawer->logic();
    worldDrawer->logic();
  }

  fn draw() noexcept -> void
  {
    BeginDrawing();
    ClearBackground(Color{ 52, 48, 56, 255 });

    camera.draw([this] {
      worldDrawer->drawTiles();
      entityDrawer->draw();
      worldDrawer->drawObjects();

      DrawCircle(200, 200, 30, RED);
    });

    rlImGuiBegin();

    bool show = true;
    ImGui::ShowDemoWindow(&show);
    rlImGuiEnd();

    DrawCircleV(GetMousePosition(), 4, DARKGRAY);
    DrawTextEx(GetFontDefault(),
               TextFormat("[%i, %i]", GetMouseX(), GetMouseY()),
               Vector2Add(GetMousePosition(), (Vector2){ .x = -44, .y = -24 }),
               20,
               2,
               WHITE);

    DrawFPS(0, 0);
    EndDrawing();
  }
};
}
