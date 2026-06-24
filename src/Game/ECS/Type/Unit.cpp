module;
#include <BS_thread_pool.hpp>
#include <stdint.h>
#include <vector>
#include <log.hpp>
#include <windalive.hpp>
export module Game.ECS.Type.Unit;

import Math.Vector;
import Prelude.TaskQueue;
import Game.ECS.Entity;
import Game.ECS.Arrays;
import Game.ECS.Pathfinding;
import Game.ECS.Component.Physix;
import Game.ECS.Component.Transform;
import Game.ECS.Component.Attributes;
import Game.ECS.Component.Path;
import Game.ECS.Component.Vision;
import Game.ECS.Component.Behavior;
import Game.ECS.Component.Vitals;
import Game.World.State;

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
  Prelude::TaskQueue& eventQueue;

  std::vector<Entity> freeEntities;
  Entity nextEntity = 1;

public:
  Unit(BS::thread_pool<BS::tp::none>& threadPool,
       Prelude::TaskQueue& eventQueue,
       Game::ECS::Arrays& arrays,
       Game::World::State& world)
    : arrays(arrays)
    , world(world)
    , pool(threadPool)
    , eventQueue(eventQueue)
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
    eventQueue.enqueue([this, pos = std::move(pos)] {
      auto entity = create();

      arrays.transformUnit.add(entity,
                               Component::Transform{
                                 .pos = pos,
                               });

      arrays.physixUnit.add(entity, Component::Physix{});
      arrays.attributesUnit.add(entity,
                                Component::Attributes{
                                  .type = Component::Attributes::Type::Human,
                                  .index = entity,
                                });
      arrays.visionUnit.add(entity, Component::Vision{});

      auto b = Component::Behavior{
        .hunger = 255,
        .energy = 255,
        .mood = 0,
      };
      b.setDefaultPriorities();
      arrays.behaviorUnit.add(entity, std::move(b));

      arrays.vitalsUnit.add(entity,
                            Component::Vitals{
                              .health = 100,
                            });
      arrays.pathUnit.add(entity, Component::Path{});
    });
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
    eventQueue.enqueue([this, target = std::move(target)] {
      const auto& components = arrays.transformUnit.getComponents();

      const auto futures = pool.submit_blocks(
        0,
        components.size(),
        [this, target = std::move(target)](size_t start, size_t end) {
          const auto& transform = arrays.transformUnit.getComponents();
          auto& paths = arrays.pathUnit.getComponents();

          for (size_t i = start; i < end; ++i) {
            auto path = Pathfinding::findPath(world, transform[i].pos, target);
            std::reverse(path.begin(), path.end());

            paths[i] = Component::Path{ .points = std::move(path) };
          }
        });

      futures.wait();
    });
  }
};
}
}
}
