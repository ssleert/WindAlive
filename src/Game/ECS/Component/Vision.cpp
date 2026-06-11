module;
#include <array>
#include <cstdint>
export module Game.ECS.Component.Vision;

import Game.World.Object;
import Math.Vector;

using namespace Math;

namespace Game {
namespace ECS {
namespace Component {
export struct Vision
{
  struct Object
  {
    Vector2 pos;
    Game::World::Object self;
  };

  static constexpr uint8_t size = 32;

  std::array<Object, size * size> objects;
  uint16_t usedObjects;
};
}
}
}
