# SDLRaycaster

A first-person raycasting engine written from scratch in C. No game engine and
no raycasting library, just SDL2 for the window, input, image loading, and audio.
Everything from the renderer to the movement and collision system is
hand-written. It builds both as a native macOS app and as a WebAssembly page
that runs in the browser, from the same sources.

![SDLRaycaster](screenshots/screenshot_1.png)
![SDLRaycaster](screenshots/screenshot_2.png)

> **Note.** The projection is not perfectly correct, so the perspective can look a
> little off, especially while moving and looking around. Some people may find it
> causes motion sickness. Take it slow if you are sensitive to that.

## Features

**Renderer**
- Textured raycaster with per-column wall casting and a floor and ceiling caster.
- Half-height blocks that rise from the floor or hang from the ceiling, holes you
  can look down and fall into, and pillars, all driven by the map characters.
- Multithreaded rendering, the screen is split into vertical strips across the
  available CPU cores using SDL threads.
- Z-buffer for correct depth ordering across the multithreaded render.
- Adjustable field of view, render downscaling, and an optional FPS cap with
  vsync.

**Movement and camera**
- Walking, running, jumping, crouching, and crawling, each with its own eye
  height and headroom checks.
- Gravity, falling, and smooth camera height transitions.
- Mouse look plus vertical look, with full gamepad support (analog sticks for
  move and look, buttons for the actions).
- Circle-based collision against the map.

**Game**
- Five hand-made levels ("chapters") dispatched through a function-pointer table,
  with triggers that carry the player state from one level into the next.
- Textured walls, floors, and ceilings, plus footstep audio through SDL2_mixer.

## Controls

| Action | Keyboard | Gamepad |
|---|---|---|
| Look around | Mouse | Right stick |
| Move | WASD | Left stick |
| Run | Shift | L3 |
| Jump | Space | Cross |
| Crouch | C | Circle |
| Crawl | Hold C | Hold Circle |
| Quit | Escape | |

## Requirements

Built and tested on macOS. It depends on SDL2, SDL2_image, and SDL2_mixer, and
uses a macOS system call to detect CPU cores, so a Linux build would need a small
change there.

Install the dependencies with Homebrew:

```sh
brew install sdl2 sdl2_image sdl2_mixer
```

The web build only needs Emscripten, which brings its own SDL2 ports:

```sh
brew install emscripten
```

## Build and run

```sh
make        # build
make run    # build and launch
```

`make clean` removes the object files, `make fclean` also removes the binary,
`make re` rebuilds from scratch, and `make debug` builds with the address
sanitizer.

## Web build

```sh
make web    # compile to WebAssembly into web/dist/
make serve  # build and serve it at http://localhost:8000
```

`make web` compiles the same sources with Emscripten and produces a
self-contained page in `web/dist` (html, js, wasm, plus a data bundle with the
textures, sounds, and maps). Upload that folder to any static host to put the
game on a website, or embed it in a page with an iframe:

```html
<iframe src="/path/to/dist/index.html" width="960" height="620"></iframe>
```

It has to be served over HTTP, browsers will not run the wasm from a `file://`
URL. `make webclean` removes the web output. Pick another port with
`make serve WEB_PORT=9000`.

A few things differ from the native build, handled by `__EMSCRIPTEN__` guards
in the sources. The browser drives the frame timing through
requestAnimationFrame instead of the FPS cap, rendering runs on a single
thread because browser threading needs special hosting headers, the canvas
uses a fixed resolution (`WEB_WIDTH` x `WEB_HEIGHT` in the header) instead of
fullscreen, and Escape releases the mouse instead of quitting. Clicking the
canvas captures the mouse, and the page template lives in
[web/shell.html](web/shell.html).

## Layout

```
src/game_init/      window, textures, sounds, maps, entities, startup
src/game_loop/      the frame: events, updates, rendering, sound, fps
  render/           raycaster, wall and floor/ceiling casting, z-buffer
  update_entities/  movement, camera, collisions, gravity
src/controls/       keyboard and gamepad input
src/utils/          map reading and helpers
src/debug/          optional minimap and on-screen debug output
include/SDLRaycaster.h  all types, settings, and prototypes
assets/                 textures, sounds, and the five maps
web/shell.html          page template for the browser build
```

## Configuration

Everything tunable is a `#define` at the top of
[include/SDLRaycaster.h](include/SDLRaycaster.h). Change a value and rebuild with
`make re`.

**Settings**

| Define | Default | What it does |
|---|---|---|
| `DOWNSCALE` | 1 | Internal render-resolution divisor. The scene is drawn at the window size divided by this, then scaled up to fill it. 1 is full resolution, higher is faster but blockier. |
| `FOV` | 53 | Field of view, in degrees. |
| `FPS_CAP` | 121 | Upper limit on frames per second. |
| `DEFAULT_SPEED` | 2 | Base walking speed. Running raises it. |
| `MOUSE_SENSIT` | 0.03 | Mouse look sensitivity. |
| `JOY_SENSIT` | 2 | Gamepad stick look sensitivity. |
| `GRAVITY` | 80 | Strength of gravity applied while falling. |
| `COLLISION_RADIUS` | 0.3 | Player collision radius, in map cells. |
| `VSYNC` | TRUE | Sync rendering to the display refresh rate. |
| `WEB_WIDTH` / `WEB_HEIGHT` | 960 x 540 | Canvas resolution of the web build. |

**Debug flags** (all off by default)

| Define | What it does |
|---|---|
| `START_LEVEL` | Which level (0 to 4) to start on. |
| `SHOW_MINIMAP` | Draw the top-down minimap overlay. |
| `MINIMAP_BLOCK` | Size of each minimap cell, in pixels. |
| `SHOW_FPS` | Print the average FPS to the terminal twice a second. |
| `SHOW_POSITION` | Print the player position each frame. |
| `SHOW_DIRECTION` | Print the player facing vector each frame. |
| `SHOW_CAM_PLANE` | Print the camera plane vector each frame. |
| `PRINT_ENTITIES` | Print the parsed entities once at startup. |
| `PRINT_MAPS` | Print the parsed maps once at startup. |

## Credits

Wall, floor, and ceiling textures are from the
[Tiny Texture Pack 2](https://screamingbrainstudios.itch.io/tiny-texture-pack-2)
by Screaming Brain Studios.

## License

MIT, see [LICENSE](LICENSE).
