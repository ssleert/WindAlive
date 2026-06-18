#include <cstdlib>
#include <log.hpp>
#include <memory>
#include <thread>
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

  engine = std::make_unique<Game::Engine>();

  auto stop = false;
  auto engineTickerThread = std::thread([&engine, &stop] {
    using Clock = std::chrono::steady_clock;
    using DurationMs = std::chrono::milliseconds;
    auto lastTime = Clock::now();

    // Statistics tracking
    auto secondStart = Clock::now();
    auto totalTickTime = DurationMs(0);
    int tickCount = 0;

    while (!stop) {
      auto currentTime = Clock::now();
      lastTime = currentTime;

      auto tickStart = Clock::now();
      engine->tick();
      auto tickEnd = Clock::now();

      auto tickDuration =
        std::chrono::duration_cast<DurationMs>(tickEnd - tickStart);

      totalTickTime += tickDuration;
      ++tickCount;

      auto frameTime =
        std::chrono::duration_cast<DurationMs>(Clock::now() - currentTime);

      if (frameTime < DurationMs(1000 / 60)) {
        std::this_thread::sleep_for(DurationMs(1000 / 60) - frameTime);
      }

      auto now = Clock::now();
      if (std::chrono::duration_cast<DurationMs>(now - secondStart) >=
          DurationMs(1000)) {
        if (tickCount > 0) {
          double avgTickMs =
            static_cast<double>(totalTickTime.count()) / tickCount;

          SPDLOG_INFO("[Engine] Average tick time: {:.2f} ms ({} ticks/sec)",
                      avgTickMs,
                      tickCount);
        }

        // Reset for next second
        secondStart = now;
        totalTickTime = DurationMs{ 0 };
        tickCount = 0;
      }
    }
  });

  window.loop();
  stop = true;
  engineTickerThread.join();
  return 0;
}
