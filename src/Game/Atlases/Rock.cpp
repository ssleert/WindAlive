module;
#include "spdlog/spdlog.h"
#include <raylib.h>
#include <stdint.h>
#include <vector>
#include <windalive.hpp>
export module Game.Atlases.Rock;

import Game.World.Tile;
import Game.World.Object;

namespace Game {
namespace Atlases {
export class Rock
{
private:
  const int32_t width = 64;
  const int32_t height = 64;

  const std::vector<Vector2> mapping = { { 0, 0 } };

public:
  fn GetTextureRectangle(Game::World::Object obj) const -> Rectangle
  {
    if constexpr (!NDEBUG_VAR) {
      if (!obj.isRock()) {
        SPDLOG_WARN("Game::World:Object with value = {} is not a Rock.",
                    (int)obj.type);
      }
    }

    auto indexTile = mapping[(int32_t)(obj.type - Game::World::Object::Rock0) %
                             mapping.size()];
    return {
      .x = (float)(indexTile.x * width),
      .y = (float)(indexTile.y * height),
      .width = (float)width,
      .height = (float)height,
    };
  }
};
}
}
