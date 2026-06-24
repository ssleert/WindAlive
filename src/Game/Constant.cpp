module;
#include <cstddef>
export module Game.Constant;

namespace Game {
export struct Constant
{
  // idk, maybe tuned in future
  // i set to 60 just because it looks smooth
  // maybe i need to add interpolation of movement in rendering
  static constexpr size_t Tickrate = 60;
};
}
