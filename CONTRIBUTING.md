# Contributing to Cinder

Thanks for your interest in contributing! This document covers everything you need to know to write code that fits Cinder's style and structure.

---

## Building the Project

```bash
git clone https://github.com/UniquePython/cinder.git
cd cinder
cmake -B build
cmake --build build -j$(nproc)
```

Make sure the SDL2 dependencies are installed first — see the README for platform-specific instructions.

---

## Code Style

Cinder is written in C11. There is no enforced formatter, so the rule is simple: **match what's already there**.

### Naming

- **Types** use `PascalCase` with a `Cinder` prefix — `CinderSound`, `CinderWindowDesc`.
- **Functions** use `PascalCase` with a `Cinder` prefix — `CinderLoadTexture`, `CinderBeginFrame`.
- **Enum values and macros** use `SCREAMING_SNAKE_CASE` with a `CINDER_` prefix — `CINDER_STATUS_OK`, `CINDER_LOG_ERROR`, `CINDER_COLOR_RGBA`.
- **Internal/private identifiers** that should not be called directly use a trailing double underscore — `CinderLogInternal__`.
- **Static helper functions** inside `.c` files have no fixed style.
- **Global state** lives in a single struct `gCinderCtx` — don't add new global variables.

### Functions

- Every public function in a header must have a full Doxygen comment block with `@brief`, `@param`, `@return`, and `@see` where applicable.
- Convenience wrappers (e.g. `CinderDrawCircleC`, `CinderDrawLineP`) are thin one-liners that delegate to the primary function — keep them that way.
- Guard against NULL inputs at the top of every function; return early or return a safe zero-value rather than crashing.
- Use `CINDER_LOG(CINDER_LOG_ERROR, "...")` for errors and `CINDER_LOG(CINDER_LOG_WARNING, "...")` for non-fatal issues.

### Types

- Prefer Cinder's own types (`CinderPoint`, `CinderRect`, `CinderVec2f`, etc.) over raw integers or SDL types in public APIs.
- New public types go in `include/cinder_types.h` if they are shared across modules, or in the relevant module header if they are module-specific.
- Use anonymous structs inside unions to allow both named access (`rect.x`, `rect.w`) and array access (`rect.pos`, `rect.size`), following the pattern in `cinder_types.h`.

### Memory

- Every allocation must have a matching free path. Follow the `Destroy` pattern — take a `**ptr`, free it, and set the caller's pointer to `NULL`.
- Check every `malloc` return value; log and return `NULL` on failure.
- Don't leak SDL resources — `SDL_FreeSurface`, `SDL_DestroyTexture`, etc. must always be called on the right path.

### Headers

- All public headers use include guards of the form `CINDER_<MODULE>_H_`.
- Internal headers (`cinder_internal.h`) must not be included from public headers.
- Keep `cinder_internal.h` as the single place that pulls in SDL2, stdlib, and other implementation dependencies — `.c` files include it and nothing else.

### File structure

```
include/cinder_<module>.h   — public API
src/cinder_<module>.c       — implementation
src/cinder_internal.h       — internal shared state, SDL2 includes
```

Adding a new module means adding both files and updating `include/cinder.h` to include the new header.

---

## Documentation

Every new public function, type, enum, and macro needs a Doxygen comment. Follow the style in the existing headers — `@brief` on the first line, then `@param`/`@return`/`@note`/`@see` as appropriate. Section dividers like `// ------- CIRCLE -------` are used to group related functions within a header; add yours in the right place.

---

## Pull Requests

Before submitting, make sure:

- The project builds cleanly with no warnings.
- You've tested on at least one supported platform (Linux or macOS).
- New public functions are documented in their header.
- The examples in `examples/` still build and run correctly.
- If you added a new module or API surface, the table in `README.md` is updated.

Fill out the PR template — it exists for a reason.

---

## What Counts as In Scope

Cinder is intentionally minimal. A good contribution fits at least one of these:

- Fixes a real bug in the existing API.
- Adds a well-scoped primitive that fills a genuine gap (e.g. a missing draw function, a missing math utility).
- Improves documentation, examples, or CI.

Cinder is **not** a game engine. PRs adding scenes, entity systems, asset pipelines, or networking will be closed. When in doubt, open an issue first.