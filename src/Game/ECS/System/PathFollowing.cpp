module;
#include <BS_thread_pool.hpp>
#include <log.hpp>
#include <stdint.h>
#include <vector>
#include <windalive.hpp>
export module Game.ECS.System.PathFollowing;

import Game.ECS.ComponentArray;
import Game.ECS.Component.Transform;
import Game.ECS.Component.Physix;
import Game.ECS.Component.Path;
import Math.Vector;

using namespace Math;

namespace Game {
namespace ECS {
namespace System {

export class PathFollowing
{
private:
  BS::thread_pool<BS::tp::none>& pool;

public:
  PathFollowing(BS::thread_pool<BS::tp::none>& threadPool)
    : pool(threadPool)
  {
  }

  fn apply(ComponentArray<Component::Transform>& transform,
           ComponentArray<Component::Physix>& physix,
           ComponentArray<Component::Path>& pathArray) -> void
  {
    const auto& pathComponents = pathArray.getComponents();
    if (pathComponents.empty())
      return;

    pool.detach_blocks(0, pathComponents.size(), [&](size_t start, size_t end) {
      const auto& transforms = transform.getComponents();
      auto& physixes = physix.getComponents();
      auto& paths = pathArray.getComponents();

      for (size_t i = start; i < end; ++i) {
        const auto& transform = transforms[i];
        auto& physix = physixes[i];
        auto& path = paths[i];

        if (path.points.empty()) {
          continue;
        }

        const auto& lastPoint = path.points.back();
        auto div = lastPoint - transform.pos;

        const auto length = div.length();
        if (length < 10.0f) {
          path.points.pop_back();

          if (path.points.empty()) {
            physix.velocity = Vector2{ 0, 0 };
            continue;
          }

          const auto& lastPoint = path.points.back();

          div = lastPoint - transform.pos;
        }

        physix.velocity = div.normalize();
      }
    });
  }
};
}
}
}
