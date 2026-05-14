<p align="center">
  <img src="assets/kitra_1024x1024.png" width="256" alt="Kitra logo">
</p>

# Kitra

A minimal, beginner-friendly 2D graphics library for C.

[![Version](https://img.shields.io/github/v/release/UniquePython/kitra?color=orange)](https://github.com/UniquePython/kitra/releases)
[![CI](https://github.com/UniquePython/kitra/actions/workflows/ci.yml/badge.svg)](https://github.com/UniquePython/kitra/actions/workflows/ci.yml)
[![License: MIT](https://img.shields.io/badge/License-MIT-orange.svg)](LICENSE)
[![Platform](https://img.shields.io/badge/platform-Linux%20%7C%20macOS-red.svg)](README.md)
[![C Standard](https://img.shields.io/badge/C-C11-red.svg)](CMakeLists.txt)
[![SDL2](https://img.shields.io/badge/built%20on-SDL2-orange.svg)](https://www.libsdl.org/)
[![Docs](https://img.shields.io/badge/docs-github.io-red.svg)](https://uniquepython.github.io/kitra)

## What is Kitra

SDL2 is a powerful foundation, but it asks you to manage renderers, surfaces, and event queues before you can draw a circle. Kitra sits on top of SDL2 and collapses that setup into a single header and a straightforward game loop, so a beginner can focus on building something instead of configuring something. It is not a game engine — there are no scenes, no entities, no editor — just a clean API for a window, input, and drawing.

## Getting Started

A Kitra program looks like this:

```c
#include <kitra/kitra.h>

int main(void)
{
    if (KitraInit(KITRA_SUBSYSTEM_ALL) != KITRA_STATUS_OK)
        return 1;

    if (KitraCreateWindow(KitraDefaultWindowDesc()) != KITRA_STATUS_OK)
        return 1;

    while (KitraIsRunning())
    {
        KitraBeginFrame();

        KitraClearBackground(KITRA_BLACK);

        KitraEndFrame();
    }

    KitraQuit(); // cleans up window, renderer, and SDL
    return 0;
}
```

## Building & Installation

> Tested on Linux and macOS. Windows support is experimental —
> MSVC has stricter C compliance requirements that may require source changes.

### Dependencies

Install the required libraries before building.

```bash
# Ubuntu/Debian
sudo apt install libsdl2-dev libsdl2-image-dev libsdl2-gfx-dev libsdl2-ttf-dev libsdl2-mixer-dev

# Arch
sudo pacman -S sdl2 sdl2_image sdl2_gfx sdl2_ttf sdl2_mixer
```

### Building Kitra

```bash
git clone https://github.com/UniquePython/kitra.git
cd kitra
cmake -B build
cmake --build build -j$(nproc)
sudo cmake --install build
```

### Using Kitra in Your Project

**CMake:**
```cmake
find_package(Kitra REQUIRED)
target_link_libraries(your_target PRIVATE Kitra::Kitra)
```

**Makefile / manual:**
```makefile
CFLAGS  += $(shell pkg-config --cflags kitra)
LIBS    += $(shell pkg-config --libs kitra)
```

**Raw gcc:**
```bash
gcc main.c $(pkg-config --cflags --libs kitra) -o myapp
```

Then include the header:

```c
#include <kitra/kitra.h>
```

## API Overview

| Module | Header | Description |
|---|---|---|
| Subsystem | `kitra/kitra_subsystem.h` | Initialize and shut down Kitra subsystems |
| Window | `kitra/kitra_window.h` | Create and destroy the application window |
| Loop | `kitra/kitra_loop.h` | Game loop control — begin frame, end frame, quit |
| Input | `kitra/kitra_input.h` | Keyboard and mouse state, per-frame and held queries |
| Drawing | `kitra/kitra_draw.h` | Lines, circles, triangles, rectangles |
| Textures | `kitra/kitra_texture.h` | Load, draw, and destroy image textures |
| Audio | `kitra/kitra_audio.h` | Load and play sound effects and music |
| Blend Modes | `kitra/kitra_blend.h` | Set blend modes globally or per-texture — none, alpha, additive, multiply |
| Text & Fonts | `kitra/kitra_text.h` | Load fonts, draw text, cache rendered text |
| Timers | `kitra/kitra_timer.h` | Delta time, FPS, and general-purpose timers |
| RNG | `kitra/kitra_rng.h` | PCG32 random number generator — integers, floats, directions, and a global convenience API |
| Math & Types | `kitra/kitra_math.h` | Vectors, points, sizes, rectangles, circles and math operations. Linkable without SDL as `Kitra::Math` |
| Plugins | `kitra/kitra_plugin.h` | Register and manage lifecycle plugins with init, update, draw, and shutdown callbacks |
| Error & Logging | `kitra/kitra_error.h` | Error state, log callbacks, and the `KITRA_LOG` macro |

## Examples

The `examples/` directory contains Pong, Snake and Pendulum built with Kitra.

<figure align="center">
  <img src="examples/pong/assets/screenshot.png" alt="Pong game in Kitra" width="700">
  <figcaption><em>Figure 1: Pong game in Kitra</em></figcaption>
</figure>

<figure align="center">
  <img src="examples/snake/assets/screenshot.png" alt="Snake game in Kitra" width="700">
  <figcaption><em>Figure 2: Snake game in Kitra</em></figcaption>
</figure>

<figure align="center">
  <img src="examples/pendulum/assets/screenshot.png" alt="Single Pendulum simulation in Kitra" width="700">
  <figcaption><em>Figure 3: Single Pendulum simulation in Kitra</em></figcaption>
</figure>

## License

Kitra is licensed under the [MIT License](LICENSE).

## Acknowledgements

Kitra was heavily inspired by raylib and its philosophy of making graphics programming simple and accessible.