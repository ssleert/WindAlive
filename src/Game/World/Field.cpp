module;
#include <stdint.h>
#include <windalive.hpp>
export module Game.World.Field;

import Game.World.Tile;
import Game.World.Object;

namespace Game {
namespace World {
export struct Field
{
  int32_t x;
  int32_t y;
  Game::World::Tile tile;
  Game::World::Object object;

  fn isCollidable() const noexcept -> bool
  {
    return tile.value == Game::World::Tile::Rock0 ||
           object.value != Game::World::Object::None;
  }
};
}
}
