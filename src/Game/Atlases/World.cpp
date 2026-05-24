module;
#include <raylib.h>
#include <stdint.h>
#include <vector>
#include <windalive.hpp>
export module Game.Atlases.World;

import Game.World.Tile;

namespace Game {
namespace Atlases {
export class World
{
private:
  const int32_t width = 64;
  const int32_t height = 64;

  const std::vector<Vector2> mapping = { { 0, 0 },
                                         { 1, 0 },
                                         { 2, 0 },
                                         { 3, 0 },
                                         { 4, 0 } };

public:
  fn GetTextureRectangle(Game::World::Tile index) const -> Rectangle
  {
    auto indexTile = mapping[(int32_t)index % mapping.size()];
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
