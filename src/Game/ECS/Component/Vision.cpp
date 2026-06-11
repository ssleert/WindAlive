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
    Game::World::Object::Value type;
  };

  static constexpr int32_t Side = 16;
  static constexpr int32_t Size = Side * Side;

  std::array<Object, Size> objects;
  uint16_t usedObjects;
};
}
}
}
