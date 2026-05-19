# AGENTS.md - WindAlive Codebase

This document captures **non-obvious** knowledge required to work effectively in the WindAlive codebase. It assumes the agent can read individual files and focuses on architecture, flow, conventions, gotchas, and tooling that are not immediately apparent from a single source file.

## Project Overview

WindAlive is a 2D top-down tile-based prototype built with Raylib and C++26 modules. It features:
- A large procedurally generated world (currently 1024×1024 random tiles).
- Texture atlas rendering with culling.
- Pannable/zoomable camera.
- Strict separation between `Application` (window/loop) and `Game` (simulation/rendering).

The project heavily uses C++ modules (`.cpp` files exporting `module` declarations).

## Tooling & Commands

### Build System

The project uses a hybrid CMake + Makefile setup:

```bash
make generate    # Configures CMake (Ninja, Debug by default, OpenGL 4.3)
make build       # Builds with Ninja (parallel)
make clean       # Removes build directory
```

- **Default compiler**: `clang++` (set via `CC`/`CXX` in Makefile).
- **Build directory**: `./build`
- **Compile commands**: Generated (`build/compile_commands.json`).
- Key CMake flags set:
  - C++26 (`CMAKE_CXX_STANDARD 26`)
  - IPO/LTO enabled
  - `-ffast-math -march=native -mtune=native`
  - Raylib with `OPENGL_VERSION=4.3`

**Important**: Modules are collected via `file(GLOB_RECURSE)` in `CMakeLists.txt` and added as a `FILE_SET CXX_MODULES`. Adding a new module file requires re-running `make generate` or manually refreshing CMake.

### Running

After build:
```bash
./build/windalive
```

No command-line arguments are currently used.

### Logging

- spdlog with `windalive.hpp` defining `SPDLOG_ACTIVE_LEVEL` based on `NDEBUG`.
- Raylib logs are redirected to spdlog via `SetTraceLogCallback`.
- Debug builds: `LOG_ALL`; Release: `LOG_WARNING`.

## Architecture & Control Flow

### High-Level Structure

```
main()
  └─ Application::Window (template)
       ├─ input()  lambda → Engine::input()
       ├─ logic()  lambda → Engine::logic()
       └─ draw()   lambda → Engine::draw()

Game::Engine
  ├─ owns World::State (generated)
  ├─ owns WorldDrawer
  ├─ owns Camera (Camera2D)
  └─ owns TexturesLoader (RAII)
```

### Module Dependencies (import graph highlights)

- `Application.Window` — top-level, depends on nothing game-specific.
- `Game.Engine` — orchestrates everything, imports many modules.
- `Game.WorldDrawer` — performs frustum culling using current `Camera2D` values.
- `Game.Camera` — mutates its internal `Camera2D`; provides `draw(Function&&)` RAII wrapper (`BeginMode2D`/`EndMode2D`).

### Data Flow

1. **World Generation**: `Game::World::Generator` creates flat `std::vector<Field>` (no 2D grid, just linear list).
2. **Rendering**: `WorldDrawer::draw()` does simple AABB culling against projected camera bounds, then `DrawTexturePro` from atlas.
3. **Camera**: 
   - `input()`: drag panning (left mouse).
   - `logic()`: zoom (mouse wheel) + sets `offset = mouse position` (unusual centering behavior).

**Note**: Camera logic is split across `input()` and `logic()` in a non-obvious way. Zoom and mouse world position updates happen in `logic()`.

## Code Organization

All source is in `src/` (flat, no subdirectories beyond the module files themselves).

Modules are named with `export module X.Y.Z;` style (namespace hierarchy reflected in module names).

Header-only style include guard: `WINDALIVE_HPP` with `#define fn auto`.

## Naming & Style Conventions

- **Modules**: `Application.*`, `Game.*`, `Game.World.*`, `Game.Atlases.*`
- **Functions**: `fn name() -> return_type` (macro in `windalive.hpp`)
- **Types**: `PascalCase` for classes/structs/enums.
- **Members**: `camelCase` or `snake_case` mixed (mostly camelCase).
- **Raylib types**: Used directly (`Vector2`, `Rectangle`, `Camera2D`, etc.).
- **Constants**: Often `const int32_t` members.

### Lambda Capture Patterns

In `main()` the `Window` is constructed with lambdas capturing `engine` by reference. `engine` is allocated *after* the `Window` object — this works because the lambdas are only invoked inside `loop()`, but it is fragile.

## Important Gotchas & Non-Obvious Behaviors

1. **Engine Allocation Order**:
   - `Application::Window` is constructed first.
   - `Game::Engine* engine;` is declared before.
   - `engine = new Game::Engine(...)` happens *after* `Window` construction.
   - Lambdas close over the pointer — valid only because execution is later. Changing initialization order can break things.

2. **WorldDrawer Culling**:
   - Culling math assumes camera `offset` and `target` are in a specific state (updated in `logic()`).
   - Uses screen-space projection — sensitive to when `logic()` vs `draw()` is called.

3. **Camera Behavior**:
   - `camera.offset = GetMousePosition()` every frame in `logic()`.
   - `camera.target = mouseWorldPos` — this creates a "mouse-centered" feel rather than traditional free camera.

4. **Module + CMake Fragility**:
   - Any new `.cpp` file must be picked up by the `GLOB` in `CMakeLists.txt`.
   - Module interface changes can cause obscure compilation errors due to C++26 module support being relatively new/unstable in some toolchains.

5. **Random Generation**:
   - `std::srand(123123)` in `main()` — deterministic for now.
   - `Generator` fills `width × height` but loop uses `world.height` for both x and y (potential bug if width != height).

6. **Texture Atlas**:
   - Hardcoded 64×64 tiles, mapping only first 5 tiles in `Game::Atlases::World`.
   - `Tile` enum has more values than the mapping array — modulo used.

7. **Performance**:
   - Full world vector (1M+ `Field` structs) iterated every frame (though culled).
   - No spatial partitioning yet.

## Development Patterns

- **RAII**: `TexturesLoader` loads/unloads texture. `Window` handles init/shutdown implicitly via Raylib.
- **Template Window**: Generic over input/logic/draw callables — allows flexible composition.
- **Const correctness**: Many `draw()` methods are `const` (good practice observed).
- **No tests**: No test directory or CMake test setup visible.

## When Modifying

- Adding new systems: Follow `Game::` module hierarchy and import style.
- Changing camera: Understand split `input()`/`logic()` responsibilities.
- World changes: Update both `Generator` and `WorldDrawer` culling logic together.
- New assets: Update `TexturesLoader` and atlas mapping.

This document will evolve as the codebase grows. Always verify module dependencies and rebuild configuration after structural changes.
```
