module;
#include <stdint.h>
#include <vector>
#include <windalive.hpp>
export module Game.World.State;

import Game.World.Field;

namespace Game {
  namespace World {
    export struct State {
      int32_t width;
      int32_t height;
      int32_t fieldSize;

      std::vector<Game::World::Field> fields;
    };
  }
}
