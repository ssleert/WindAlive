module;
#include <stdint.h>
#include <windalive.hpp>
export module Game.World.Field;

import Game.World.Tile;

namespace Game {
  namespace World {
    export struct Field {
      int32_t x;
      int32_t y;
      Game::World::Tile tile;
    };
  }
}
