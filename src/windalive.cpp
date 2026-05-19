#include <thread>

#include <log.hpp>
#include <cstdlib>
#include <windalive.hpp>

import Application.Window;
import Game.Engine;
import Game.TexturesLoader;

fn main() -> int {
  SPDLOG_INFO("windalive");

  std::srand(123123);

  const int32_t width = 1920;
  const int32_t height = 1080;

  Game::Engine* engine;
  auto window = Application::Window(width, height,
    [&]{
      engine->input();
    }, 
    [&]{
      engine->logic();
    },
    [&]{
      engine->draw();
    }
  );

  engine = new Game::Engine(width, height);

  window.loop();
  delete engine;
  return 0;
}
