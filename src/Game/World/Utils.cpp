module;
#include <algorithm>
#include <cstdint>
#include <utility>
#include <windalive.hpp>
export module Game.World.Utils;

import Game.World.State;
import Math.Vector;

using namespace Math;

namespace Game {
namespace World {

export class Utils
{
public:
  fn WorldToField(const Vector2& worldPos, const Game::World::State& state)
    -> Vector2
  {
    if (state.fieldSize <= 0) {
      return { 0, 0 };
    }

    auto fieldX = (int32_t)(worldPos.x / state.fieldSize);
    auto fieldY = (int32_t)(worldPos.y / state.fieldSize);

    fieldX = std::clamp(fieldX, 0, state.width - 1);
    fieldY = std::clamp(fieldY, 0, state.height - 1);

    return { (float)fieldX, (float)fieldY };
  }

  fn FieldToWorld(int32_t fieldX,
                  int32_t fieldY,
                  const Game::World::State& state) -> Vector2
  {
    float worldX = (float)(fieldX * state.fieldSize);
    float worldY = (float)(fieldY * state.fieldSize);

    return Vector2{ worldX, worldY };
  }

  fn FieldToWorld(const Vector2& coord, const Game::World::State& state)
    -> Vector2
  {
    return FieldToWorld(coord.x, coord.y, state);
  }
};

}
}
