module;
#include <BS_thread_pool.hpp>
#include <log.hpp>
#include <vector>
#include <windalive.hpp>
export module Game.ECS.System.Behavior;

import Game.ECS.Entity;
import Game.ECS.ComponentArray;
import Game.ECS.Component.Vision;
import Game.ECS.Component.Behavior;
import Game.ECS.Component.Attributes;
import Game.ECS.Component.Transform;
import Game.ECS.Component.Path;
import Game.World.Object;

namespace Game {
namespace ECS {
namespace System {
export class Behavior
{
private:
  BS::thread_pool<BS::tp::none>& pool;

public:
  Behavior(BS::thread_pool<BS::tp::none>& threadPool)
    : pool(threadPool)
  {
  }

  // TODO: refactor
  fn apply(const ComponentArray<Component::Transform>& transform,
           const ComponentArray<Component::Vision>& vision,
           const ComponentArray<Component::Attributes>& attributes,
           ComponentArray<Component::Behavior>& behavior,
           ComponentArray<Component::Path>& path) -> void
  {
    const auto& components = transform.getComponents();
    pool.detach_blocks(
      0, components.size(), [&](const size_t start, const size_t end) {
        const auto& tc = transform.getComponents();
        const auto& vc = vision.getComponents();
        const auto& ac = attributes.getComponents();
        auto& bc = behavior.getComponents();
        auto& pc = path.getComponents();

        for (size_t i = start; i < end; ++i) {
          Tick(bc[i], tc[i], vc[i], pc[i], ac[i].index);
        }
      });
  }

  fn Tick(Component::Behavior& bc,
          const Component::Transform& tc,
          const Component::Vision& vc,
          Component::Path& pc,
          Entity entity) -> void
  {
    // TODO
    return;
  }
};
}
}
}
