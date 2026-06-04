module;
#include <log.hpp>
#include <raylib.h>
#include <stdint.h>
#include <vector>
#include <windalive.hpp>
export module Game.EntityDrawer;

import Game.Camera;
import Game.ECS.State;
import Math.Vector;

namespace Game {
export class EntityDrawer
{
private:
  int32_t screenWidth;
  int32_t screenHeight;
  const Game::Camera& camera;
  Game::ECS::State& entities;

public:
  EntityDrawer(Game::ECS::State& entities, const Game::Camera& camera)
    : camera(camera)
    , entities(entities)
  {
  }

  fn input() -> void
  {
    if (IsMouseButtonDown(MOUSE_BUTTON_RIGHT)) {
      const auto mpos =
        GetScreenToWorld2D(GetMousePosition(), camera.getCamera());
      entities.unit.setDestinationAll(Math::Vector2{ mpos.x, mpos.y });
    }
    if (IsMouseButtonDown(MOUSE_BUTTON_LEFT)) {
      const auto mpos =
        GetScreenToWorld2D(GetMousePosition(), camera.getCamera());
      entities.unit.add(Math::Vector2{ mpos.x, mpos.y });
    }
  }

  fn logic() -> void
  {
    screenWidth = GetScreenWidth();
    screenHeight = GetScreenHeight();
  }

  // TODO: add occlusion culling
  fn draw() const noexcept -> void
  {
    for (size_t e = 0; e < entities.arrays.transformUnit.getComponents().size();
         ++e) {
      auto t = entities.arrays.transformUnit.getComponents()[e];
      DrawRectangleRec(
        Rectangle{
          .x = t.pos.x,
          .y = t.pos.y,
          .width = 30,
          .height = 30,
        },
        WHITE);
    }
  }
};
}
