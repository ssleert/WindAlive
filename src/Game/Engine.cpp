module;
#include <memory>
#include <raylib.h>
#include <raymath.h>
#include <rlgl.h>
#include <stdint.h>
#include <vector>
#include <windalive.hpp>
export module Game.Engine;

import Game.Camera;
import Game.TexturesLoader;
import Game.Atlases.World;
import Game.Atlases.Tree;
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
  const int32_t width;
  const int32_t height;

  const Game::TexturesLoader texturesLoader;
  const Game::Atlases::World worldAtlas;
  const Game::Atlases::Tree treeAtlas;

  Game::Camera camera;

  // TODO: rewrite to late initialization
  std::unique_ptr<Game::WorldDrawer> worldDrawer;
  std::unique_ptr<Game::EntityDrawer> entityDrawer;

  Game::World::State world;
  Game::ECS::State entities;

public:
  Engine(int32_t width, int32_t height)
    : width(width)
    , height(height)
    , entities(world)
  {
    auto generator = Game::World::Generator(1024, 1024);
    world = generator.generate();

    worldDrawer = std::make_unique<Game::WorldDrawer>(
      width, height, world, texturesLoader, worldAtlas, treeAtlas, camera);

    entityDrawer =
      std::make_unique<Game::EntityDrawer>(width, height, entities, camera);
  }

  fn tick() -> void
  {
    // TODO: move to server
    entities.tick();
  }

  fn input() -> void
  {
    entityDrawer->input();
    camera.input();
  }

  fn logic() -> void
  {
    camera.logic();
    worldDrawer->logic();
  }

  fn draw() const noexcept -> void
  {
    BeginDrawing();
    ClearBackground(Color{ 52, 48, 56, 255 });

    camera.draw([this] {
      worldDrawer->drawTiles();
      entityDrawer->draw();
      worldDrawer->drawObjects();

      DrawCircle(200, 200, 30, RED);
    });

    DrawCircleV(GetMousePosition(), 4, DARKGRAY);
    DrawTextEx(GetFontDefault(),
               TextFormat("[%i, %i]", GetMouseX(), GetMouseY()),
               Vector2Add(GetMousePosition(), (Vector2){ -44, -24 }),
               20,
               2,
               WHITE);

    DrawFPS(0, 0);
    EndDrawing();
  }
};
}
