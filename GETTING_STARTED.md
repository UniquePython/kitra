# Getting Started

## What Cinder Sets Up For You

Drawing something to the screen in C requires more infrastructure than it
might seem. Under the hood, your operating system needs to create a window,
allocate a surface to draw on, and set up a pipeline to get pixels from your
program onto the display. SDL2 handles all of this, but it asks you to manage
the pieces yourself — initializing subsystems, creating a window handle,
creating a renderer attached to that window, and keeping track of all of it.

Cinder wraps that setup into two calls:

```c
if (CinderInit(CINDER_SUBSYSTEM_ALL) != CINDER_STATUS_OK)
    return 1;

if (CinderCreateWindow(CinderDefaultWindowDesc()) != CINDER_STATUS_OK)
    return 1;
```

`CinderInit` starts the SDL2 subsystems you need — events, audio, video.
`CINDER_SUBSYSTEM_ALL` is the right default for most programs. If
initialization fails for any reason, it returns a non-OK status and logs the
error automatically. `CinderCreateWindow` creates the window and the renderer
attached to it. `CinderDefaultWindowDesc()` gives you a 900×600 centered
window with a sensible title — you can customize any field before passing it
in.

Both functions return a `CinderStatus`. Always check it. A failure here means
nothing else will work.

## The Game Loop

A graphics program doesn't run top-to-bottom like a command-line tool. It
runs in a loop — typically 60 times per second — where each iteration reads
input, updates state, and draws the current frame. This is called the game
loop, and it is the fundamental structure of every interactive graphics
program.

In Cinder, the loop looks like this:

```c
while (CinderIsRunning())
{
    CinderBeginFrame();

    // update and draw here

    CinderEndFrame();
}
```

`CinderIsRunning()` returns true until something calls `CinderRequestQuit()`
— either your own code, or Cinder internally when the user closes the window.
You never call `while(1)` directly because you need a clean way to exit the
loop from anywhere in your program.

`CinderBeginFrame()` does three things at the start of each iteration: it
drains the OS event queue (processing window close, keyboard, and mouse
events), updates the input state so that `CinderIsKeyDown` and
`CinderIsKeyPressed` reflect the current frame, and calculates delta time —
the number of seconds since the last frame. Everything that reads input or
calls `CinderGetDeltaTime()` must happen after `CinderBeginFrame()`.

`CinderEndFrame()` does the opposite bookkeeping at the end: it presents the
finished frame to the screen, and if you've set a target FPS with
`CinderSetTargetFPS()`, it sleeps for the remaining time to avoid burning
CPU unnecessarily.

Nothing you draw will appear on screen until `CinderEndFrame()` is called.

## Why You Clear Every Frame

You might wonder why the starter program calls `CinderClearBackground` every
frame instead of just drawing once. The reason is how rendering works under
the hood.

Cinder uses double buffering. At any moment there are two framebuffers: the
one currently displayed on screen, and the one you are drawing into. When you
call `CinderEndFrame()`, those two buffers swap — the one you just drew into
becomes the display, and the old display becomes your next drawing surface.

That drawing surface is not automatically cleared between frames. Whatever you
drew last frame is still there. If you move a circle and don't clear first,
you'll see a trail of every position it has ever occupied. So the correct
pattern is always:

```c
CinderBeginFrame();

CinderClearBackground(CINDER_BLACK); // wipe the previous frame
// draw everything for this frame
// ...

CinderEndFrame();
```

Think of it like animation on paper — you don't draw the next frame on top of
the last one, you start with a blank sheet every time.

## Quitting Cleanly

`CinderIsRunning()` returns false when a quit has been requested. There are
two ways this happens:

The first is automatic. When the user clicks the window's close button, SDL2
generates a quit event. `CinderBeginFrame()` processes that event internally
and calls `CinderRequestQuit()` on your behalf. You don't need to handle
window close yourself.

The second is manual. You can call `CinderRequestQuit()` anywhere in your
code — typically in response to a keypress:

```c
if (CinderIsKeyPressed(CINDER_KEY_ESCAPE))
    CinderRequestQuit();
```

This doesn't immediately exit the loop. It sets an internal flag that makes
`CinderIsRunning()` return false on the next check, so the current frame
finishes cleanly before the loop exits.

After the loop, always call `CinderQuit()`:

```c
CinderQuit(); // cleans up window, renderer, and SDL
return 0;
```

`CinderQuit()` destroys the window, the renderer, and shuts down SDL2. If you
allocated fonts or textures, free those before calling `CinderQuit()` — after
it returns, the renderer they depend on no longer exists.

## What Comes Next

That's the complete skeleton of a Cinder program. From here, everything builds
on the same structure — `CinderBeginFrame`, update, draw, `CinderEndFrame`.

The next things worth learning:

- **Input** — `CinderIsKeyDown` vs `CinderIsKeyPressed`, and how to read mouse
  position and buttons
- **Drawing** — lines, circles, rectangles, and how the coordinate system works
- **Textures and text** — loading images and fonts, and the difference between
  `CinderDrawText` and `CinderCreateText`

The `examples/` directory contains Pong and Snake built with Cinder. Reading it alongside
this guide is the fastest way to see how all the pieces fit together in a real
program.