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

  // TODO: find a way for enum class methods
  // looks terrible :(
  static fn IsTree(Value v) -> bool { return v == Tree0; }
  static fn IsRock(Value v) -> bool { return v == Rock0; }
  static fn Exist(Value v) -> bool { return v != None; }

  Object(uint8_t value, uint8_t xDiff, uint8_t yDiff)
    : value((Value)value)
    , xDiff(xDiff)
    , yDiff(yDiff)
  {
  }

  Object()
    : value(Value::None)
    , xDiff(0)
    , yDiff(0)
  {
  }

  fn isTree() const -> bool { return IsTree(value); }
  fn isRock() const -> bool { return IsRock(value); }
  fn exist() const -> bool { return Exist(value); }

  Value value;
  uint8_t xDiff;
  uint8_t yDiff;
};
}
}
