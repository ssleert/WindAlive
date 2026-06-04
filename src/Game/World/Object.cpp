module;
#include <stdint.h>
#include <windalive.hpp>
export module Game.World.Object;

namespace Game {
namespace World {
export class Object
{
public:
  enum Value : uint8_t
  {
    None,
    Tree0,
    Rock0,
    Size,
  };

  Object(uint8_t value, uint8_t xDiff, uint8_t yDiff)
    : value((Value)value)
    , xDiff(xDiff)
    , yDiff(yDiff)
  {
  }

  Object()
    : value(None)
    , xDiff(0)
    , yDiff(0)
  {
  }

  fn isTree() const -> bool { return value == Tree0; }
  fn isRock() const -> bool { return value == Rock0; }

  fn exist() const -> bool { return value != None; }

  Value value;
  uint8_t xDiff;
  uint8_t yDiff;
};
}
}
