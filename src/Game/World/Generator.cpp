module;
#include <stdint.h>
#include <vector>
#include <cstdlib>
#include <windalive.hpp>
export module Game.World.Generator;

import Game.World.Tile;
import Game.World.Field;
import Game.World.State;

namespace Game {
  namespace World {
    export class Generator {
      private:
        int32_t width;
        int32_t height;

      public:
        Generator(
          int32_t width,
          int32_t height
        ) : width(width), 
            height(height) 
        {} 

        fn generate() const -> Game::World::State {
          auto world = Game::World::State{
            .width = width,
            .height = height,
            .fieldSize = 64,

            .fields = std::vector<Game::World::Field>(),
          };

          for (int32_t y = 0; y < world.height; ++y) {
            for (int32_t x = 0; x < world.height; ++x) {
              world.fields.push_back(Game::World::Field{
                .x = x,
                .y = y,
                .tile = (Game::World::Tile)(std::rand() % (int)Game::World::Tile::Size),
              });
            }
          }

          return world;
        }
    };
  }
}
