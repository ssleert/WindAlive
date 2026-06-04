module;
#include <raylib.h>
export module Game.TexturesLoader;

namespace Game {
export class TexturesLoader
{
public:
  Texture2D worldTiles;
  Texture2D treeTiles;
  Texture2D rockTiles;

  TexturesLoader()
  {
    worldTiles = LoadTexture("./asset/world.png");
    treeTiles = LoadTexture("./asset/tree.png");
    rockTiles = LoadTexture("./asset/rock.png");
  }

  ~TexturesLoader()
  {
    UnloadTexture(worldTiles);
    UnloadTexture(treeTiles);
    UnloadTexture(rockTiles);
  }
};
}
