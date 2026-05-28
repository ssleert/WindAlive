module;
#include <exception>
#include <log.hpp>
#include <raylib.h>
#include <stdint.h>
#include <windalive.hpp>
export module Application.Window;

namespace Application {
export template<class InputFunction, class LogicFunction, class DrawFunction>
class Window
{
private:
  static void RaylibLogCallback(int logLevel, const char* text, va_list args)
  {
    char buffer[1024];
    vsnprintf(buffer, sizeof(buffer), text, args);

    switch (logLevel) {
      case LOG_TRACE:
        SPDLOG_TRACE("{}", buffer);
        break;
      case LOG_DEBUG:
        SPDLOG_DEBUG("{}", buffer);
        break;
      case LOG_INFO:
        SPDLOG_INFO("{}", buffer);
        break;
      case LOG_WARNING:
        SPDLOG_WARN("{}", buffer);
        break;
      case LOG_ERROR:
        SPDLOG_ERROR("{}", buffer);
        break;
      case LOG_FATAL:
        SPDLOG_CRITICAL("{}", buffer);
        break;
      default:
        SPDLOG_INFO("{}", buffer);
        break;
    }
  }

  InputFunction input;
  LogicFunction logic;
  DrawFunction draw;

public:
  int32_t width = 0;
  int32_t height = 0;
  int32_t fps = 10000;
  const char* title = "WindAlive - 0.0.1 alpha";

  Window(int32_t width,
         int32_t height,
         DrawFunction&& draw,
         LogicFunction&& logic,
         InputFunction&& input)
    : width(width)
    , height(height)
    , input(input)
    , logic(logic)
    , draw(draw)
  {
    SetTraceLogCallback(RaylibLogCallback);

    if constexpr (NDEBUG_VAR) {
      SetTraceLogLevel(LOG_WARNING);
    } else {
      SetTraceLogLevel(LOG_ALL);
    }

    InitWindow(width, height, title);
    SetTargetFPS(fps);
  }

  ~Window() { CloseWindow(); }

  fn loop() -> void
  {
    while (!WindowShouldClose()) {
      try {
        input();
        logic();
      } catch (const std::exception& err) {
        SPDLOG_ERROR("UNHANDLED EXCEPTION: {}", err.what());
      } catch (...) {
        SPDLOG_ERROR("UNHANDLED UNKNOWN EXCEPTION");
      }

      draw();
    }
  }
};
}
