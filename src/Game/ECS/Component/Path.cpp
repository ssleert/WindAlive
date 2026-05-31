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
  std::vector<Vector2> points;
};
}
}
}
