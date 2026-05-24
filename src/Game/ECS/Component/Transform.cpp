module;
export module Game.ECS.Component.Transform;

import Math.Vector;

using namespace Math;

namespace Game {
namespace ECS {
namespace Component {
export struct Transform
{
  Vector2 pos;
  float rotation;
};
}
}
}
