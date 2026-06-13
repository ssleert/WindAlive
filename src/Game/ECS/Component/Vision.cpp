module;
#include <array>
#include <cstdint>
#include <utility>
#include <windalive.hpp>
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
    Game::World::Object::Type type;
  };

  static constexpr int32_t Side = 16;
  static constexpr int32_t Size = Side * Side;

  std::array<Object, Size> objects;
  uint16_t usedObjects;

  template<class Function>
  fn findNearestObject(Function func) const noexcept
    -> std::pair<const Object&, bool>
  {
    for (size_t i = 0; i < usedObjects; ++i) {
      const auto& obj = objects[i];
      if (func(obj)) {
        return { obj, true };
      }
    }

    return { objects[0], false };
  }
};
}
}
}
