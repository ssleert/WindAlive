module;
#include <vector>
export module Game.ECS.Component.Path;

import Math.Vector;

using namespace Math;

namespace Game {
namespace ECS {
namespace Component {
export struct Path
{
  Vector2 finalPoint;
  bool completed;

  std::vector<Vector2> points;
};
}
}
}
