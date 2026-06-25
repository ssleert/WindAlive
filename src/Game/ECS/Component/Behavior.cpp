module;
#include <boost/container/static_vector.hpp>
#include <cstdint>
#include <windalive.hpp>
export module Game.ECS.Component.Behavior;

import Math.Vector;

using namespace Math;

namespace Game {
namespace ECS {
namespace Component {
export struct Behavior
{
  struct Task
  {
    enum class Type : uint8_t
    {
      Free,
      GetTree,
      GetRock,
      Build,
      Repair,
      Sleep,
      Eat,
      Fight,
      Patrol,
      Idle,
      Size
    };

    Type type;
    uint8_t priority;
  };

  static constexpr size_t MaxPriorities = 16;
  boost::container::static_vector<Task::Type, MaxPriorities> tasksPriorities;
  Task currentTask;

  uint8_t hunger;
  uint8_t energy;
  int8_t mood; // minus vibe)

  fn setDefaultPriorities() noexcept -> void
  {
    tasksPriorities = {
      Task::Type::Fight,   Task::Type::Repair,  Task::Type::Build,
      Task::Type::GetRock, Task::Type::GetTree, Task::Type::Eat,
      Task::Type::Sleep,   Task::Type::Patrol,  Task::Type::Idle,
    };
  }
};
}
}
}
