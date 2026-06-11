module;
#include <stdint.h>
#include <windalive.hpp>
export module Game.ECS.Arrays;

import Game.ECS.ComponentArray;
import Game.ECS.Component.Physix;
import Game.ECS.Component.Transform;
import Game.ECS.Component.Attributes;
import Game.ECS.Component.Path;
import Game.ECS.Component.Vision;
import Game.ECS.Component.Behavior;
import Game.ECS.Component.Vitals;

namespace Game {
namespace ECS {
export struct Arrays
{
  ComponentArray<Component::Transform> transformUnit;
  ComponentArray<Component::Physix> physixUnit;
  ComponentArray<Component::Attributes> attributesUnit;
  ComponentArray<Component::Vision> visionUnit;
  ComponentArray<Component::Behavior> behaviorUnit;
  ComponentArray<Component::Vitals> vitalsUnit;
  ComponentArray<Component::Path> pathUnit;
};
}
}
