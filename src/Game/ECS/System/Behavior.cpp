module;
#include <BS_thread_pool.hpp>
#include <log.hpp>
#include <vector>
#include <windalive.hpp>
export module Game.ECS.System.Behavior;

// TODO: calc all formules independly of tickrate

import Game.ECS.Entity;
import Game.ECS.ComponentArray;
import Game.ECS.Component.Vision;
import Game.ECS.Component.Behavior;
import Game.ECS.Component.Attributes;
import Game.ECS.Component.Transform;
import Game.ECS.Component.Path;
import Game.World.Object;
import Game.Constant;

namespace Game {
namespace ECS {
namespace System {
export class Behavior
{
private:
  static constexpr auto Tickrate = Game::Constant::Tickrate;

  // (60 * 60 * 24 * 3) / 255
  // 3 days to die
  static constexpr uint16_t hungerTick = (Tickrate * 60 * 24 * 3) / 255;

  // 16 * 60 * 60 / 255
  // 16 hours work day
  static constexpr uint16_t energyTick = (Tickrate * 60 * 16) / 255;

  // (150 / 1000) = x / 256
  static constexpr uint16_t minTaskPriority = (255 * 0.150f);

  // minHunger = 255 * 15% = 38.25 => 38
  static constexpr uint16_t minHunger = (255 * 0.15f);

  // minEnergy = 255 * 20% = 51
  static constexpr uint16_t minEnergy = (255 * 0.20f);

  BS::thread_pool<BS::tp::none>& pool;
  uint16_t hungerTicker;
  uint16_t energyTicker;

  fn finishCurrentTask(Component::Behavior& bc, Component::Path& pc) noexcept
    -> void
  {
    pc.points.clear();
    bc.currentTask = {};
  }

  fn cancelCurrentTask(Component::Behavior& b, Component::Path& pc) noexcept
    -> void
  {
    pc.points.clear();
    b.currentTask = {};
  }

  fn updateCurrentTask(Component::Behavior& bc,
                       const Component::Transform& tc,
                       const Component::Vision& vc,
                       Component::Path& pc,
                       Entity entity) -> bool
  {
    // TODO
    return true;
  }

  fn startCurrentTask(Component::Behavior& bc,
                      const Component::Transform& tc,
                      const Component::Vision& vc,
                      Component::Path& pc,
                      Entity entity) -> void
  {
    // TODO
    return;
  }

  fn shouldInterrupt(const Component::Behavior& bc) const noexcept -> bool
  {
    // TODO: do another findBestTask and compare

    if (bc.currentTask.priority < minTaskPriority) {
      return true;
    }

    return bc.hunger < minHunger || bc.energy < minEnergy;
  }

  fn generateTask(Component::Behavior::Task::Type type,
                  const Component::Behavior& bc,
                  const Component::Transform& tc,
                  const Component::Vision& vc) -> Component::Behavior::Task
  {
    auto task = Component::Behavior::Task{
      .type = type,
    };

    using Type = Component::Behavior::Task::Type;
    switch (type) {
      case Type::GetTree:
        //const auto [obj, found] = v.find
      default:
        return {}; // just task with 0 priority
    }

    return task;
  }

  fn findBestTask(Component::Behavior& bc,
                  const Component::Transform& tc,
                  const Component::Vision& vc) -> Component::Behavior::Task
  {
    auto best = Component::Behavior::Task();
    float bestScore = -1.0f;

    for (const auto& type : bc.tasksPriorities) {
      auto task = generateTask(type, bc, tc, vc);
      return {};
    }

    return best;
  }

  fn tick(Component::Behavior& bc,
          const Component::Transform& tc,
          const Component::Vision& vc,
          Component::Path& pc,
          Entity entity) -> void
  {
    if (hungerTicker % hungerTick) {
      if (bc.hunger != 0) {
        bc.hunger--;
      }
    }

    if (bc.hunger == 0) {
      // TODO: kill him or make motivation 0
    }

    if (energyTicker % energyTick) {
      bc.energy--;
    }

    if (bc.currentTask.type != Component::Behavior::Task::Type::Free) {
      if (shouldInterrupt(bc)) {
        cancelCurrentTask(bc, pc);
      } else if (updateCurrentTask(bc, tc, vc, pc, entity)) {
        finishCurrentTask(bc, pc);
        return;
      } else {
        return;
      }
    }

    // TODO: find best task
  }

public:
  Behavior(BS::thread_pool<BS::tp::none>& threadPool)
    : pool(threadPool)
    , hungerTicker(0)
    , energyTicker(0)
  {
  }

  // TODO: refactor
  fn apply(const ComponentArray<Component::Transform>& transform,
           const ComponentArray<Component::Vision>& vision,
           const ComponentArray<Component::Attributes>& attributes,
           ComponentArray<Component::Behavior>& behavior,
           ComponentArray<Component::Path>& path) -> void
  {
    // TODO: maybe queue ticker updates to eventQueue in ecs state
    hungerTicker++;
    energyTicker++;

    const auto& components = transform.getComponents();
    pool.detach_blocks(
      0, components.size(), [&](const size_t start, const size_t end) {
        const auto& tc = transform.getComponents();
        const auto& vc = vision.getComponents();
        const auto& ac = attributes.getComponents();
        auto& bc = behavior.getComponents();
        auto& pc = path.getComponents();

        for (size_t i = start; i < end; ++i) {
          tick(bc[i], tc[i], vc[i], pc[i], ac[i].index);
        }
      });
  }
};
}
}
}
