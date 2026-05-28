module;
#include <stdint.h>
#include <vector>
#include <windalive.hpp>
export module Game.ECS.Arrays;

import Game.ECS.ComponentArray;
import Game.ECS.Component.Physix;
import Game.ECS.Component.Transform;
import Game.ECS.Component.Attributes;
import Game.ECS.Component.Path;

namespace Game {
namespace ECS {
export struct Arrays
{
  ComponentArray<Component::Transform> transformUnit;
  ComponentArray<Component::Physix> physixUnit;
  ComponentArray<Component::Attributes> attributesUnit;
  ComponentArray<Component::Path> pathUnit;
};
}
}
