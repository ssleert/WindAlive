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

namespace Game {
namespace ECS {
namespace System {
export class Behavior
{
private:
  // (60 * 60 * 24 * 3) / 255
  // 3 days to die
  static constexpr size_t hungerTick = 1012;

  // 16 * 60 * 60 / 255
  // 16 hours work day
  static constexpr size_t energyTick = 225;

  BS::thread_pool<BS::tp::none>& pool;
  uint16_t hungerTicker;
  uint16_t energyTicker;

  fn finishCurrentTask(Component::Behavior& bc, Component::Path& pc) noexcept
    -> void
  {
    pc.points.clear();
    bc.currentTask = {};
  }

  fn shouldInterrupt(const Component::Behavior& bc) const noexcept -> bool
  {
    // TODO: do another findBestTask and compare

    // (150 / 1000) = x / 256
    // x = 38.4 => 38
    if (bc.currentTask.priority < 38) {
      return true;
    }

    // minHunger = 255 * 15% = 38.25 => 38
    // minEnergy = 255 * 20% = 51
    return bc.hunger < 38 || bc.energy < 51;
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

    // TODO: task execution
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
