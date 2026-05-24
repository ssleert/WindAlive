#include <memory>
#include <thread>

#include <cstdlib>
#include <log.hpp>
#include <windalive.hpp>

import Application.Window;
import Game.Engine;
import Game.TexturesLoader;

fn
main() -> int
{
  std::srand(123123);

  const int32_t width = 1920;
  const int32_t height = 1080;

  std::unique_ptr<Game::Engine> engine;
  auto window = Application::Window(
    width,
    height,
    [&engine] { engine->input(); },
    [&engine] { engine->logic(); },
    [&engine] { engine->draw(); });

  engine = std::make_unique<Game::Engine>(width, height);

  auto stop = false;
  auto engineTickerThread = std::thread([&engine, &stop] {
    using Clock = std::chrono::steady_clock;
    using FloatSeconds = std::chrono::duration<float>;

    constexpr float targetDeltaTime = 1.0f / 60.0f;

    auto lastTime = Clock::now();

    while (!stop) {
      auto currentTime = Clock::now();
      float deltaTime =
        std::chrono::duration_cast<FloatSeconds>(currentTime - lastTime)
          .count();
      lastTime = currentTime;

      engine->tick();

      auto frameTime = std::chrono::duration_cast<std::chrono::milliseconds>(
        Clock::now() - currentTime);

      if (frameTime < std::chrono::milliseconds(1000 / 60)) {
        std::this_thread::sleep_for(std::chrono::milliseconds(1000 / 60) -
                                    frameTime);
      }
    }
  });

  window.loop();
  stop = true;
  engineTickerThread.join();
  return 0;
}
