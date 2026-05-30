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
           ComponentArray<Component::Path>& pathArray,
           float speed = 130.0f) -> void
  {
    const auto& pathComponents = pathArray.getComponents();
    if (pathComponents.empty())
      return;

    pool.detach_blocks(0, pathComponents.size(), [&, speed=speed](size_t start, size_t end) {
      auto& transforms = transform.getComponents();
      auto& physixes = physix.getComponents();
      auto& paths = pathArray.getComponents();

      for (size_t i = start; i < end; ++i) {
        auto& p = paths[i];
        auto& t = transforms[i];
        auto& ph = physixes[i];

        if (p.completed || p.points.empty()) {
          ph.velocity = { 0, 0 };
          continue;
        }

        Vector2 target = p.points.front();
        Vector2 dir = target - t.pos;
        float distSq = dir.lengthSquared();

        // Reached current waypoint
        if (distSq < 64.0f) // 8 pixels tolerance
        {
          p.points.erase(p.points.begin());

          if (p.points.empty()) {
            p.completed = true;
            ph.velocity = { 0, 0 };
            continue;
          }

          target = p.points.front();
          dir = target - t.pos;
          distSq = dir.lengthSquared();
        }

        if (distSq > 0.001f) {
          dir = dir.normalize();
          ph.velocity = dir * speed;
        } else {
          ph.velocity = { 0, 0 };
        }
      }
    });
  }
};
}
}
}
