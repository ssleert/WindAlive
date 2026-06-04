module;
#include <BS_thread_pool.hpp>
#include <stdint.h>
#include <windalive.hpp>
export module Game.ECS.State;

import Prelude.TaskQueue;
import Math.Vector;
import Game.ECS.Arrays;
import Game.ECS.Entity;
import Game.ECS.System.Movement;
import Game.ECS.System.PathFollowing;
import Game.ECS.Type.Unit;
import Game.World.State;

using namespace Math;

namespace Game {
namespace ECS {
export class State
{
private:
  BS::thread_pool<BS::tp::none> pool;
  Prelude::TaskQueue eventQueue;

public:
  ECS::Arrays arrays;
  System::Movement movementSystem;
  System::PathFollowing pathFollowingSystem;
  Type::Unit unit;

  Game::World::State& world;

  State(Game::World::State& world)
    : pool(std::thread::hardware_concurrency)
    , movementSystem(pool)
    , pathFollowingSystem(pool)
    , world(world)
    , unit(pool, eventQueue, arrays, world)
  {
  }

  fn tick() noexcept -> void
  {
    // execute events from Engine thread
    eventQueue.wait();

    pool.wait();
    pathFollowingSystem.apply(
      arrays.transformUnit, arrays.physixUnit, arrays.pathUnit);
    pool.wait();
    movementSystem.apply(arrays.transformUnit, arrays.physixUnit);
    pool.wait();
  }
};
}
}
