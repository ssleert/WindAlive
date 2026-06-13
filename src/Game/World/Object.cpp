module;
#include <stdint.h>
#include <windalive.hpp>
export module Game.World.Object;

namespace Game {
namespace World {
export class Object
{
public:
  enum Type : uint8_t
  {
    None,
    Tree0,
    Rock0,
    Size,
  };

  // TODO: find a way for enum class methods
  // looks terrible :(
  static fn IsTree(Type v) -> bool { return v == Tree0; }
  static fn IsRock(Type v) -> bool { return v == Rock0; }
  static fn Exist(Type v) -> bool { return v != None; }

  Object(uint8_t type, uint8_t xDiff, uint8_t yDiff)
    : type((Type)type)
    , xDiff(xDiff)
    , yDiff(yDiff)
  {
  }

  Object()
    : type(Type::None)
    , xDiff(0)
    , yDiff(0)
  {
  }

  fn isTree() const -> bool { return IsTree(type); }
  fn isRock() const -> bool { return IsRock(type); }
  fn exist() const -> bool { return Exist(type); }

  Type type;
  uint8_t xDiff;
  uint8_t yDiff;
};
}
}
