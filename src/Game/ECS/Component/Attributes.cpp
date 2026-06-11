module;
#include <stdint.h>
export module Game.ECS.Component.Attributes;

namespace Game {
namespace ECS {
namespace Component {
export struct Attributes
{
  enum Type : uint8_t
  {
    Entity,
    Human,
  };

  Type type;
  uint8_t variation;
};
}
}
}
