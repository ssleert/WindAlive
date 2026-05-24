module;
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
  const int32_t screenWidth;
  const int32_t screenHeight;
  const Game::Camera& camera;
  Game::ECS::State& entities;

public:
  EntityDrawer(int32_t screenWidth,
               int32_t screenHeight,
               Game::ECS::State& entities,
               const Game::Camera& camera)
    : camera(camera)
    , entities(entities)
    , screenWidth(screenWidth)
    , screenHeight(screenHeight)
  {
  }

  fn input() -> void
  {
    if (IsMouseButtonDown(MOUSE_BUTTON_RIGHT)) {
      auto mpos = GetScreenToWorld2D(GetMousePosition(), camera.getCamera());
      entities.addPointOfVelocity(Math::Vector2{ mpos.x, mpos.y });
    }
    if (IsMouseButtonDown(MOUSE_BUTTON_LEFT)) {
      auto mpos = GetScreenToWorld2D(GetMousePosition(), camera.getCamera());
      entities.addHuman(Math::Vector2{ mpos.x, mpos.y });
    }
  }

  // TODO: add occlusion culling
  fn draw() const noexcept -> void
  {
    for (const auto& e : entities.transformAlive.getEntities()) {
      auto t = entities.transformAlive[e];
      DrawRectangleRec(
        Rectangle{
          .x = t.pos.x,
          .y = t.pos.y,
          .width = 60,
          .height = 60,
        },
        WHITE);
    }
  }
};
}
