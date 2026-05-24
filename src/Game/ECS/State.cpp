module;
#include <BS_thread_pool.hpp>
#include <stdint.h>
#include <vector>
#include <windalive.hpp>
export module Game.ECS.State;

import Game.ECS.ComponentArray;
import Game.ECS.Component.Physix;
import Game.ECS.Component.Transform;
import Game.ECS.Component.Attributes;
import Game.ECS.Entity;
import Game.ECS.System.Movement;

import Math.Vector;

using namespace Math;

namespace Game {
namespace ECS {
export class State
{
public:
  ComponentArray<Component::Transform> transformAlive;
  ComponentArray<Component::Physix> physixAlive;
  ComponentArray<Component::Attributes> attributesAlive;

  System::Movement movementSystem;

  Entity nextEntity = 1;
  std::vector<Entity> freeEntities;

  BS::thread_pool<BS::tp::none> threadPool;

  State()
    : threadPool(std::thread::hardware_concurrency)
  {
  }

  fn createEntity() -> Entity
  {
    if (!freeEntities.empty()) {
      Entity e = freeEntities.back();
      freeEntities.pop_back();

      return e;
    }

    return nextEntity++;
  }

  fn destroyEntity(Entity e) -> void
  {
    transformAlive.remove(e);
    physixAlive.remove(e);
    attributesAlive.remove(e);
  }

  fn addHuman(Vector2 pos) -> Entity
  {
    auto entity = createEntity();

    transformAlive.add(entity,
                       Component::Transform{
                         .pos = pos,
                       });

    physixAlive.add(entity, Component::Physix{});
    attributesAlive.add(entity,
                        Component::Attributes{
                          .type = Component::Attributes::Type::Human,
                        });

    return entity;
  }

  fn addPointOfVelocity(Vector2 pos) -> void
  {
    for (const auto& e : physixAlive.getEntities()) {
      auto& p = physixAlive[e];
      const auto& t = transformAlive[e];

      auto sub = pos - t.pos;
      p.velocity = sub / sub.length();
    }
  }

  fn tick() noexcept -> void
  {
    movementSystem.apply(transformAlive, physixAlive);
  }
};
}
}
