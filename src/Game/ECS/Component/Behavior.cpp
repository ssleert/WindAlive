module;
#include <array>
#include <cstdint>
export module Game.ECS.Component.Behavior;

namespace Game {
namespace ECS {
namespace Component {
export struct Behavior
{
  enum class Task : uint8_t
  {
    Free,
    GetTree,
    GetRock,
    Build,
  };

  enum State : uint8_t
  {
    Idle,
    Sleeping,
    Working,
  };

  State state;

  // i think 32 is still to much :(
  std::array<Task, 32> tasks;
  uint8_t usedTasks;
};
}
}
}
