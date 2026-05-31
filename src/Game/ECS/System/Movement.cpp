module;
#include <BS_thread_pool.hpp>
#include <vector>
#include <windalive.hpp>
export module Game.ECS.System.Movement;

import Game.ECS.ComponentArray;
import Game.ECS.Component.Transform;
import Game.ECS.Component.Physix;

namespace Game {
namespace ECS {
namespace System {
export class Movement
{
private:
  BS::thread_pool<BS::tp::none>& pool;

public:
  Movement(BS::thread_pool<BS::tp::none>& threadPool)
    : pool(threadPool)
  {
  }

  fn apply(ComponentArray<Component::Transform>& transform,
           const ComponentArray<Component::Physix>& physix) -> void
  {
    const auto& components = transform.getComponents();
    pool.detach_blocks(
      0, components.size(), [&](const size_t start, const size_t end) {
        auto& tc = transform.getComponents();
        const auto& pc = physix.getComponents();
        for (size_t i = start; i < end; ++i) {
          auto& t = tc[i];
          const auto& p = pc[i];
          t.pos += p.velocity * 10;
        }
      });
  }
};
}
}
}
