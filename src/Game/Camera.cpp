module;
#include <cmath>
#include <raylib.h>
#include <raymath.h>
#include <rlgl.h>
#include <windalive.hpp>
export module Game.Camera;

namespace Game {
export class Camera
{
private:
  Camera2D camera;

  float wheel;
  Vector2 mouseWorldPos;

public:
  Camera()
  {
    camera = {
      .zoom = 1.0f,
    };
  }

  fn getCamera() const -> const Camera2D& { return camera; }

  fn input() -> void
  {
    wheel = GetMouseWheelMove();

    if (IsMouseButtonDown(MOUSE_BUTTON_MIDDLE)) {
      Vector2 delta = GetMouseDelta();
      delta = Vector2Scale(delta, -1.0f / camera.zoom);
      camera.target = Vector2Add(camera.target, delta);
    }
  }

  fn logic() -> void
  {
    const auto mousePos = GetMousePosition();
    mouseWorldPos = GetScreenToWorld2D(mousePos, camera);

    camera.offset = mousePos;
    camera.target = mouseWorldPos;

    // TODO: issues with object flex on screen while zooming
    float scale = 0.2f * wheel;
    camera.zoom = Clamp(expf(logf(camera.zoom) + scale), 0.125f, 64.0f);

    camera.zoom = std::round(camera.zoom * 100.f) / 100.f;
  }

  template<class Function>
  fn draw(Function&& callback) const noexcept -> void
  {
    BeginMode2D(camera);
    callback();
    EndMode2D();
  }
};
}
