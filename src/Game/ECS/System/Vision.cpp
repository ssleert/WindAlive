module;
#include <BS_thread_pool.hpp>
#include <algorithm>
#include <log.hpp>
#include <windalive.hpp>
export module Game.ECS.System.Vision;

import Game.ECS.ComponentArray;
import Game.ECS.Component.Transform;
import Game.ECS.Component.Vision;
import Game.World.State;
import Math.Vector;

using namespace Math;

namespace Game {
namespace ECS {
namespace System {
export class Vision
{
private:
  BS::thread_pool<BS::tp::none>& pool;
  Game::World::State& world;

public:
  Vision(BS::thread_pool<BS::tp::none>& threadPool, Game::World::State& world)
    : pool(threadPool)
    , world(world)
  {
  }

  fn apply(ComponentArray<Component::Transform>& transform,
           ComponentArray<Component::Vision>& vision) -> void
  {
    const auto& components = transform.getComponents();

    pool.detach_blocks(
      0, components.size(), [&](const size_t start, const size_t end) {
        const auto& tc = transform.getComponents();
        auto& vc = vision.getComponents();
        const float fieldSizeF = (float)world.fieldSize;
        const int32_t halfVision = Component::Vision::Side / 2;

        for (size_t i = start; i < end; ++i) {
          const auto& t = tc[i];
          auto& v = vc[i];

          int32_t gx = (int32_t)t.pos.x / fieldSizeF;
          int32_t gy = (int32_t)t.pos.y / fieldSizeF;

          v.usedObjects = 0;
          const uint16_t capacity = Component::Vision::Size;

          for (int32_t dy = -halfVision; dy < halfVision; ++dy) {
            for (int32_t dx = -halfVision; dx < halfVision; ++dx) {
              int32_t nx = gx + dx;
              int32_t ny = gy + dy;

              if (nx < 0 || nx >= world.width || ny < 0 || ny >= world.height)
                continue;

              const auto& field = world.fields[ny * world.width + nx];
              if (!field.object.exist())
                continue;

              // TODO: remove after tests
              if (v.usedObjects >= capacity)
                break; // safety, though unlikely

              v.objects[v.usedObjects] = Component::Vision::Object{
                .pos = { .x = (float)nx * fieldSizeF + fieldSizeF * 0.5f,
                         .y = (float)ny * fieldSizeF + fieldSizeF * 0.5f },
                .type = field.object.value,
              };

              ++v.usedObjects;
            }

            if (v.usedObjects >= capacity)
              break;
          }

          std::sort(v.objects.begin(),
                    v.objects.begin() + v.usedObjects,
                    [&](const Component::Vision::Object& a,
                        const Component::Vision::Object& b) {
                      float distA = (a.pos - t.pos).lengthSquared();
                      float distB = (b.pos - t.pos).lengthSquared();
                      return distA < distB;
                    });
        }
      });
  }
};
}
}
}
