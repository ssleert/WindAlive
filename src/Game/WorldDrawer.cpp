module;
#include <raylib.h>
#include <stdint.h>
#include <vector>
#include <windalive.hpp>
export module Game.WorldDrawer;

import Game.World.State;
import Game.World.Field;
import Game.TexturesLoader;
import Game.Atlases.World;
import Game.Atlases.Tree;
import Game.Camera;

namespace Game {
export class WorldDrawer
{
private:
  const int32_t screenWidth;
  const int32_t screenHeight;

  const Game::World::State& world;
  const Game::TexturesLoader& texturesLoader;
  const Game::Atlases::World& worldAtlas;
  const Game::Atlases::Tree& treeAtlas;
  const Game::Camera& camera;

  float left;
  float top;
  float right;
  float bottom;

  float w;
  float h;

  std::vector<const Game::World::Field*> fieldsWithObjects;

public:
  WorldDrawer(int32_t screenWidth,
              int32_t screenHeight,
              const Game::World::State& world,
              const Game::TexturesLoader& texturesLoader,
              const Game::Atlases::World& worldAtlas,
              const Game::Atlases::Tree& treeAtlas,
              const Game::Camera& camera)
    : world(world)
    , texturesLoader(texturesLoader)
    , worldAtlas(worldAtlas)
    , treeAtlas(treeAtlas)
    , camera(camera)
    , screenWidth(screenWidth)
    , screenHeight(screenHeight)
  {
    fieldsWithObjects.reserve(world.width * world.height * 0.4f);

    w = (float)world.fieldSize;
    h = (float)world.fieldSize;
  }

  fn logic() -> void
  {
    const auto& c = camera.getCamera();

    left = (0 - c.offset.x) / c.zoom + c.target.x;
    top = (0 - c.offset.y) / c.zoom + c.target.y;
    right = (screenWidth - c.offset.x) / c.zoom + c.target.x;
    bottom = (screenHeight - c.offset.y) / c.zoom + c.target.y;
  }

  // TODO: rewrite cleaner
  fn drawTiles() noexcept -> void
  {
    fieldsWithObjects.clear();

    // TODO: calc tiles in screen space by indexes without iteration
    for (const auto& field : world.fields) {
      const float x = (float)(field.x * world.fieldSize);
      const float y = (float)(field.y * world.fieldSize);

      if (x + w > left && x < right && y + h > top && y < bottom) {
        DrawTexturePro(texturesLoader.worldTiles,
                       worldAtlas.GetTextureRectangle(field.tile),
                       (Rectangle){ .x = x, .y = y, .width = w, .height = h },
                       (Vector2){},
                       0,
                       WHITE);

        if (field.object.exist()) {
          fieldsWithObjects.push_back(&field);
        }
      }
    }
  }

  fn drawObjects() const noexcept -> void
  {
    for (const auto& field : fieldsWithObjects) {
      const float x = (float)(field->x * world.fieldSize);
      const float y = (float)(field->y * world.fieldSize);

      // TODO: move check for object type to another func
      //       and return pair of texture and rectangle
      if (field->object.isTree()) {
        DrawTexturePro(
          texturesLoader.treeTiles,
          treeAtlas.GetTextureRectangle(field->object),
          (Rectangle){
            .x = x - 5 + field->object.xDiff,
            .y = y - (float)world.fieldSize / 2 + field->object.yDiff,
            .width = w,
            .height = h * 2,
          },
          (Vector2){},
          0,
          WHITE);
      }
    }
  }
};
}
