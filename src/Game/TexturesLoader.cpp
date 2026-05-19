module;
#include <raylib.h>
export module Game.TexturesLoader;

namespace Game {
  export class TexturesLoader {
    public:
      Texture2D worldTiles; 

      TexturesLoader() {
        worldTiles = LoadTexture("./asset/world.png");
      }

      ~TexturesLoader() {
        UnloadTexture(worldTiles);
      }
  };
}
