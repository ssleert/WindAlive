module;
#include <raylib.h>
export module Game.TexturesLoader;

namespace Game {
export class TexturesLoader
{
public:
  Texture2D worldTiles;
  Texture2D treeTiles;

  TexturesLoader()
  {
    worldTiles = LoadTexture("./asset/world.png");
    treeTiles = LoadTexture("./asset/tree.png");
  }

  ~TexturesLoader() { UnloadTexture(worldTiles); }
};
}
