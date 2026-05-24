module;
#include <vector>
#include <windalive.hpp>
export module Game.ECS.System.Movement;

import Game.ECS.ComponentArray;
import Game.ECS.Component.Transform;
import Game.ECS.Component.Physix;

namespace Game {
  namespace ECS {
    namespace System {
      export class Movement {
        public:
          fn apply(
            ComponentArray<Component::Transform>& transform,
            const ComponentArray<Component::Physix>& physix
          ) -> void {
            for (const auto& e : transform.getEntities()) {
              auto& t = transform[e];
              const auto& p = physix[e];
              t.pos += p.velocity * 20;
            }
          }
      };  
    }
  }
}
