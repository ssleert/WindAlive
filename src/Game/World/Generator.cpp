module;
#include <cstdlib>
#include <log.hpp>
#include <stdint.h>
#include <vector>
#include <windalive.hpp>
export module Game.World.Generator;

import Game.World.Tile;
import Game.World.Field;
import Game.World.State;
import Game.World.Object;

namespace Game {
namespace World {
export class Generator
{
private:
  int32_t width;
  int32_t height;

public:
  Generator(int32_t width, int32_t height)
    : width(width)
    , height(height)
  {
  }

  fn generate() const -> Game::World::State
  {
    auto world = Game::World::State{
      .width = width,
      .height = height,
      .fieldSize = 64,

      .fields = std::vector<Game::World::Field>(),
    };

    for (uint16_t y = 0; y < world.height; ++y) {
      for (uint16_t x = 0; x < world.width; ++x) {
        auto idx = std::rand() % (int)Game::World::Tile::Size;

        world.fields.push_back(Game::World::Field{
          .x = x,
          .y = y,
          .tile = (Game::World::Tile)(idx),
          .object =
            Game::World::Tile(idx).forObject()
              ? Game::World::Object(std::rand() % Game::World::Object::Size,
                                    uint8_t(std::rand() % 40),
                                    uint8_t(std::rand() % 8))
              : Game::World::Object() });
      }
    }

    return world;
  }
};
}
}
