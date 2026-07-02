# SDLRaycaster

A first-person raycasting engine written from scratch in C. No game engine and
no raycasting library, just SDL2 for the window, input, image loading, and audio.
Everything from the renderer to the movement and collision system is
hand-written.

![SDLRaycaster](screenshots/screenshot.png)

## Features

**Renderer**
- Textured raycaster with per-column wall casting and a floor and ceiling caster.
- Half-height blocks that rise from the floor or hang from the ceiling, holes you
  can look down into, and pillars, all driven by the map characters.
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
- Textured walls, floors, and ceilings, plus ambient and footstep audio through
  SDL2_mixer.

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

## Build and run

```sh
make        # build
make run    # build and launch
```

`make clean` removes the object files, `make fclean` also removes the binary,
`make re` rebuilds from scratch, and `make debug` builds with the address
sanitizer.

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
```

Tuning knobs (FOV, speed, gravity, downscale, FPS cap, debug overlays) live at
the top of [include/SDLRaycaster.h](include/SDLRaycaster.h).

## License

MIT, see [LICENSE](LICENSE).
