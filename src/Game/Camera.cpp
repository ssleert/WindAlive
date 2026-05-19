module;
#include <raylib.h>
#include <rlgl.h>
#include <raymath.h>
#include <windalive.hpp>
export module Game.Camera;

namespace Game {
  export class Camera {
    private: 
      Camera2D camera;

      float wheel;
      Vector2 mousePos;
      Vector2 mouseWorldPos;
    public:
      Camera() {
        camera = {
          .zoom = 1.0f,
        };
      }

      fn getCamera() const -> const Camera2D& {
        return camera;
      }

      fn input() -> void {
        wheel = GetMouseWheelMove();

        if (IsMouseButtonDown(MOUSE_BUTTON_LEFT)) {
          Vector2 delta = GetMouseDelta();
          delta = Vector2Scale(delta, -1.0f/camera.zoom);
          camera.target = Vector2Add(camera.target, delta);
        }
      }

      fn logic() -> void {
        mouseWorldPos = GetScreenToWorld2D(GetMousePosition(), camera);
        camera.offset = GetMousePosition();
        camera.target = mouseWorldPos;

        float scale = 0.2f*wheel;
        camera.zoom = Clamp(expf(logf(camera.zoom)+scale), 0.125f, 64.0f);
      }

      template<class Function>
      fn draw(Function&& callback) const noexcept -> void { 
        BeginMode2D(camera);
          callback();
        EndMode2D();
      }
  };
}
