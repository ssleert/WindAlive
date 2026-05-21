module;
#include <stdint.h>
#include <vector>
#include <raylib.h>
#include <windalive.hpp>
export module Game.WorldDrawer;

import Game.World.State;
import Game.TexturesLoader;
import Game.Atlases.World;
import Game.Camera;

namespace Game {
  export class WorldDrawer { 
    private:
      const int32_t screenWidth;
      const int32_t screenHeight;

      const Game::World::State& world;
      const Game::TexturesLoader& texturesLoader;
      const Game::Atlases::World& worldAtlas;
      const Game::Camera& camera;

    public:
      WorldDrawer(
        int32_t screenWidth,
        int32_t screenHeight,
        const Game::World::State& world,
        const Game::TexturesLoader& texturesLoader,
        const Game::Atlases::World& worldAtlas,
        const Game::Camera& camera
      ) : world(world),
          texturesLoader(texturesLoader),
          worldAtlas(worldAtlas),
          camera(camera),
          screenWidth(screenWidth),
          screenHeight(screenHeight)
      {}

      // TODO: rewrite cleaner
      fn draw() const noexcept -> void {
        const auto& c = camera.getCamera();

        auto left = (0 - c.offset.x) / c.zoom + c.target.x;
        auto top = (0 - c.offset.y) / c.zoom + c.target.y;
        auto right = (screenWidth - c.offset.x) / c.zoom + c.target.x;
        auto bottom = (screenHeight - c.offset.y) / c.zoom + c.target.y;


        //for (const auto& field : world.fields)  {
        //  DrawTexturePro(
        //    texturesLoader.worldTiles,
        //    worldAtlas.GetTextureRectangle(field.tile),
        //    (Rectangle){
        //      .x = (float)(field.x * world.fieldSize),
        //      .y = (float)(field.y * world.fieldSize),
        //      .width = (float)world.fieldSize,
        //      .height = (float)world.fieldSize
        //    },
        //    (Vector2){},
        //    0,
        //    WHITE
        //  );
        //}
        for (const auto& field : world.fields) {
          float x = (float)(field.x * world.fieldSize);
          float y = (float)(field.y * world.fieldSize);
          float w = (float)world.fieldSize;
          float h = (float)world.fieldSize;

          if (x + w > left && x < right && y + h > top && y < bottom) {
            DrawTexturePro(
              texturesLoader.worldTiles,
              worldAtlas.GetTextureRectangle(field.tile),
              (Rectangle){
                .x = x,
                .y = y,
                .width = w,
                .height = h
              },
              (Vector2){},
              0,
              WHITE
            );
          }
        }
      }
  };
}
