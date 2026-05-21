module;
#include <stdint.h>
export module Game.ECS.Component.Health;

namespace Game {
  namespace ECS {
    namespace Component {
      export struct Health {
        int32_t health;
      };
    }
  }
}
