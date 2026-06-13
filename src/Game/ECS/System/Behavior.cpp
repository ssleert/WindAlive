module;
#include <BS_thread_pool.hpp>
#include <log.hpp>
#include <vector>
#include <windalive.hpp>
export module Game.ECS.System.Behavior;

import Game.ECS.ComponentArray;
import Game.ECS.Component.Vision;
import Game.ECS.Component.Behavior;
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
  fn apply(ComponentArray<Component::Behavior>& behavior,
           const ComponentArray<Component::Vision>& vision) -> void
  {
    const auto& components = behavior.getComponents();
    pool.detach_blocks(
      0, components.size(), [&](const size_t start, const size_t end) {
        auto& bc = behavior.getComponents();
        const auto& vc = vision.getComponents();

        for (size_t i = start; i < end; ++i) {
          auto& b = bc[i];
          const auto& v = vc[i];

          switch (b.state) {
            case Component::Behavior::State::Idle: {
              // TODO: implement Levy flight
              //       https://en.wikipedia.org/wiki/Levy_flight
              break;
            }
            case Component::Behavior::State::Sleeping: {
              // TODO: implement find of sleep place
              break;
            }
            case Component::Behavior::State::Working: {
              for (size_t taskIdx = 0; taskIdx < b.usedTasks; ++taskIdx) {
                const auto& task = b.tasks[taskIdx];

                switch (task) {
                  case Component::Behavior::Task::Free: {
                    // TODO
                    break;
                  }
                  case Component::Behavior::Task::GetTree: {
                    const auto [tree, found] =
                      v.findNearestObject([](const auto& obj) {
                        return Game::World::Object::IsTree(obj.type);
                      });

                    if (!found) {
                      break;
                    }

                    SPDLOG_INFO(
                      "nearest tree = [{},{}]", tree.pos.x, tree.pos.y);
                    break;
                  };
                  case Component::Behavior::Task::GetRock: {
                    // TODO
                    break;
                  }
                  case Component::Behavior::Task::Build: {
                    // TODO
                    break;
                  }
                }
              }
              break;
            }
          }
        }
      });
  }
};
}
}
}
