module;
#include <stdint.h>
#include <raylib.h>
#include <rlgl.h>
#include <raymath.h>
#include <vector>
#include <memory>
#include <windalive.hpp>
export module Game.Engine;

import Game.Camera;
import Game.TexturesLoader;
import Game.Atlases.World;
import Game.WorldDrawer;
import Game.World.Generator;
import Game.World.State;
import Game.ECS.State;

import Math.Vector;

namespace Game {
  export class Engine {
    private:    
      const int32_t width;
      const int32_t height;

      const Game::TexturesLoader texturesLoader;
      const Game::Atlases::World worldAtlas;

      Game::Camera camera;
      std::unique_ptr<Game::WorldDrawer> worldDrawer;

      Game::World::State world;
      Game::ECS::State entities;
    public:
      Engine(
        int32_t width,
        int32_t height
      ) : width(width), 
          height(height)
      {
        auto generator = Game::World::Generator(1024, 1024);
        world = generator.generate();

        worldDrawer = std::make_unique<Game::WorldDrawer>(
          width, height,
          world, texturesLoader, 
          worldAtlas, camera
        );
      }

      fn tick() -> void {
        // TODO: move to server
        entities.tick();
      }

      fn input() -> void {
        if (IsMouseButtonDown(MOUSE_BUTTON_RIGHT)) {
          auto mpos = GetScreenToWorld2D(GetMousePosition(), camera.getCamera());
          entities.addPointOfVelocity(Math::Vector2{mpos.x, mpos.y});
        }

        if (IsMouseButtonDown(MOUSE_BUTTON_LEFT)) {
          auto mpos = GetScreenToWorld2D(GetMousePosition(), camera.getCamera());
          entities.addHuman(Math::Vector2{mpos.x, mpos.y});
        }
        camera.input();
      }

      fn logic() -> void {
        camera.logic();
      }

      fn draw() const noexcept -> void { 
        BeginDrawing();
          ClearBackground(BLACK);

          camera.draw([this]{
            worldDrawer->draw();

            DrawCircle(200, 200, 30, RED);

            for (const auto& e : entities.transformAlive.getEntities()) {
              auto t = entities.transformAlive[e];

              DrawRectangleRec(Rectangle{
                .x = t.pos.x,
                .y = t.pos.y,
                .width = 60,
                .height = 60,
              }, WHITE);
            }
          });

          DrawCircleV(GetMousePosition(), 4, DARKGRAY);
          DrawTextEx(
            GetFontDefault(), 
            TextFormat("[%i, %i]", GetMouseX(), GetMouseY()),
            Vector2Add(
              GetMousePosition(), 
              (Vector2){ -44, -24 }
            ),
            20, 2, WHITE
          );


          DrawFPS(0, 0);
        EndDrawing();
      }
  };
}
