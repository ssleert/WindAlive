module;
#include <BS_thread_pool.hpp>
#include <stdint.h>
#include <vector>
#include <windalive.hpp>
export module Game.ECS.State;

import Game.ECS.Arrays;
import Game.ECS.Entity;
import Game.ECS.System.Movement;
import Game.ECS.Type.Unit;
import Game.World.State;
import Math.Vector;

using namespace Math;

namespace Game {
namespace ECS {
export class State
{
public:
  BS::thread_pool<BS::tp::none> pool;

  Game::World::State& world;

  ECS::Arrays arrays;

  System::Movement movementSystem;

  Type::Unit unit;

  State(

    Game::World::State& world

    )
    : pool(std::thread::hardware_concurrency)
    , movementSystem(pool)
    , world(world)
    , unit(arrays, world)
  {
  }

  fn tick() noexcept -> void
  {
    movementSystem.apply(arrays.transformUnit, arrays.physixUnit);
    pool.wait();
  }
};
}
}
