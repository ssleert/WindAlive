module;
#include <stdint.h>
#include <windalive.hpp>
export module Game.World.Tile;

namespace Game {
namespace World {
export class Tile
{
public:
  enum Value : uint8_t
  {
    Grass0,
    Grass1,
    Grass2,
    Grass3,
    Grass4,
    Grass5,
    Flower0,
    Flower1,
    Flower2,
    Flower3,
    Flower4,
    Flower5,
    Rock0,
    Size,
  };

  Tile(uint8_t value)
    : value((Value)value)
  {
  }

  Tile()
    : value(Grass0)
  {
  }

  fn forObject() -> bool { return Grass0 <= value && value <= Grass5; }

  Value value;
};
}
}
