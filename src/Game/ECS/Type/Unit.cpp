module;
#include <BS_thread_pool.hpp>
#include <log.hpp>
#include <stdint.h>
#include <vector>
#include <windalive.hpp>
export module Game.ECS.Type.Unit;

import Game.ECS.Entity;
import Game.ECS.Arrays;
import Game.ECS.Pathfinding;
import Game.ECS.Component.Physix;
import Game.ECS.Component.Transform;
import Game.ECS.Component.Attributes;
import Game.ECS.Component.Path;
import Game.World.State;
import Math.Vector;

using namespace Math;

namespace Game {
namespace ECS {
namespace Type {
export class Unit
{
private:
  Game::ECS::Arrays& arrays;
  Game::World::State& world;

  BS::thread_pool<BS::tp::none>& pool;

  std::vector<Entity> freeEntities;
  Entity nextEntity = 1;

public:
  Unit(BS::thread_pool<BS::tp::none>& threadPool,
       Game::ECS::Arrays& arrays,
       Game::World::State& world)
    : pool(threadPool)
    , arrays(arrays)
    , world(world)
  {
  }

  fn create() -> Entity
  {
    if (!freeEntities.empty()) {
      Entity e = freeEntities.back();
      freeEntities.pop_back();

      return e;
    }

    return nextEntity++;
  }

  fn add(Vector2 pos) -> void
  {
    auto entity = create();

    arrays.transformUnit.add(entity,
                             Component::Transform{
                               .pos = pos,
                             });

    arrays.physixUnit.add(entity, Component::Physix{});
    arrays.attributesUnit.add(entity,
                              Component::Attributes{
                                .type = Component::Attributes::Type::Human,
                              });
    arrays.pathUnit.add(entity, Component::Path{});
  }

  fn setDestination(Entity e, Vector2 target) -> void
  {
    auto& transform = arrays.transformUnit[e];

    // TODO: add task to threadpool
    auto path = Pathfinding::findPath(world, transform.pos, target);

    if (path.empty()) {
      // Optional: log warning
      return;
    }

    arrays.pathUnit.add(e, Component::Path{ .points = std::move(path) });
  }

  fn setDestinationAll(Vector2 target) -> void
  {
    const auto& components = arrays.transformUnit.getComponents();

    pool.detach_blocks(
      0, components.size(), [this, target = target](size_t start, size_t end) {
        const auto& transform = arrays.transformUnit.getComponents();
        auto& paths = arrays.pathUnit.getComponents();

        for (size_t i = start; i < end; ++i) {
          auto path = Pathfinding::findPath(world, transform[i].pos, target);
          std::reverse(path.begin(), path.end());

          paths[i] = Component::Path{ .points = std::move(path) };
        }
      });
  }
};
}
}
}
