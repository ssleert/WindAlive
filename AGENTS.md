# AGENTS.md

## Overview

`windalive` is a small C++ game prototype that uses **C++20‑style modules** (C++26 standard), **raylib** for rendering, **ImGui** for UI, **spdlog** for logging and a tiny **thread‑pool**.  The source lives under `src/`, third‑party libraries are in `libs/` and asset files live in `asset/`.

## Building the Project

### Using the provided Makefile
```
# Generate Ninja build files
make USE_ANGLE_FROM_CHROMIUM=ON BUILD_TYPE=Debug USE_STDCXX=ON USE_MIMALLOC=OFF  

# Build (default uses Debug with ASan/UBSan)
make build

# Clean
make clean
```

| Variable | Default | Effect |
|---|---|---|
| `BUILD_TYPE` | `Debug` | `Release` enables optimisations and disables sanitisers. |
| `DEBUGGER` | `OFF` | When `ON` the debug build does not enable ASan/UBSan. |
| `USE_ANGLE_FROM_CHROMIUM` | `OFF` | Enables EGL/EGL+GLESV2 flags for raylib. |
| `USE_STDCXX` | `OFF` | Links against libc++ on systems that provide it. |
| `USE_MIMALLOC` | `OFF` | Statically links mimalloc for allocation performance. |

### Manual CMake/Ninja
```bash
# From project root
cmake -G Ninja -B build \
  -DCMAKE_BUILD_TYPE=Debug \
  -DUSE_ANGLE_FROM_CHROMIUM=ON \
  -DUSE_STDCXX=ON \
  -DUSE_MIMALLOC=OFF \
  -DDEBUGGER=OFF
cmake --build build --parallel
```

**Tip**: Use `-DCMAKE_BUILD_TYPE=Debug` for quick builds with sanitisers.

## Running the Application

After a successful build the executable is `build/windalive`.  Run it from the project root so that relative asset paths (e.g. `./asset/world.png`) resolve:
```bash
./build/windalive
```
If you run from another directory, either copy the `asset/` directory next to the binary or set `LD_LIBRARY_PATH` accordingly.

## Code Organization

```
├── src/                 # C++ modules and game logic
├── libs/                # Third‑party libraries (raylib, spdlog, imgui, ...)
├── asset/               # PNG/TEX assets used at runtime
├── Makefile             # Convenience wrapper around CMake/Ninja
├── CMakeLists.txt       # Root CMake configuration
└── AGENTS.md            # This file
```

### Core Modules
* `Application.Window` – thin wrapper around raylib’s windowing and log callbacks.
* `Game.Engine` – main game loop, tick/logic/draw.
* `Game.World` – grid‑based world representation.
* `Game.ECS` – small entity‑component‑system implementation.

Modules are compiled as **C++ modules** (`module;` header and `export module`).  The project uses the `fn` macro to simplify function definitions:
```cpp
#define fn auto

fn foo() -> int { return 42; }
```

## Naming & Style
* Functions: defined with `fn` and return type after `->`.
* Variables: snake_case.
* Types: PascalCase (e.g., `Game::Engine`).
* Modules: dot‑separated, e.g., `Application.Window`.
* Header guards are used (`#ifndef WINDALIVE_HPP` etc.).

## Dependencies
* **raylib** – lightweight cross‑platform graphics.
* **ImGui** – immediate‑mode UI.
* **rlImGui** – ImGui integration for raylib.
* **spdlog** – high‑performance logging.
* **thread‑pool** – small header‑only thread‑pool.
* **mimalloc** – optional static allocator.

## Testing
There are currently no unit tests.  The repository focuses on the game prototype and relies on manual testing.

## Gotchas & Non‑Obvious Points
* **C++ Modules** require a compiler that supports C++20 modules (Clang 15+ or GCC 12+).  The Makefile defaults to `clang++`.
* The `Makefile` sets `-fsanitize=address,undefined` for Debug builds unless `DEBUGGER=ON`.
* Asset paths are hard‑coded relative to the working directory; always run from the project root or adjust `LoadTexture("./asset/...")` paths.
* The `-DUSE_ANGLE_FROM_CHROMIUM` flag is only relevant on Linux/Android when using EGL.
* `NDEBUG_VAR` is a compile‑time flag that switches the raylib trace log level.

## Conventions
* All source files in `src/` end with `*.cpp` and use module syntax.
* Public API is exported via `export module` and `export` keyword.
* The build system always generates a `build/` directory; never commit it.
* `Makefile` variables are used to toggle build options; modify them before invoking `make generate`.

---

> **Note**: This file is automatically updated when the repository changes.  If you add new modules or change build options, regenerate AGENTS.md.
