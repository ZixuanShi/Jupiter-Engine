# CLAUDE.md

This file provides guidance to Claude Code (claude.ai/code) when working with code in this repository.

## Project intent

JupiterEngine is a cross-platform Vulkan/Metal game engine. It compiles to a **single executable** (`JupiterEngine`) built from `Source/Main.cpp`. Target platforms: Windows, Linux, macOS, Android, iOS — using Vulkan (Win/Linux/Android) and Metal (macOS/iOS). Only windows/macos/linux are wired up in `CMakePresets.json` today; Android/iOS presets are future work.

When extending CMake, assume one binary named `JupiterEngine`. Do not introduce multi-executable abstractions or per-file `add_executable` loops.

Engine subsystems are added as C++23 modules (`export module jpt.<Name>;`, contents under `namespace jpt`). `Main.cpp` stays the single non-module entry point that imports and composes them.

A subsystem may be split across two files: the interface unit `Foo.cppm` (`export module jpt.Foo;`, declarations) and an optional implementation unit `Foo.cpp` (`module jpt.Foo;` — **no** `export`, definitions). `CMakeLists.txt` globs `Source/*.cppm` into the `CXX_MODULES` file set and `Source/*.cpp` as ordinary sources; the file set is for interface units only, so implementation units must be regular sources or their symbols never get compiled and the link fails with undefined `jpt::Foo@jpt.Foo::Bar()`.

Each translation unit needs **its own global module fragment**. A GMF is private to its file: the `#include <chrono>` in `Foo.cppm` makes those names *reachable* but not *visible* inside `Foo.cpp`, which fails with `'X' must be declared before it is used ... declaration here is not visible`. Repeat the includes in every unit that uses them.

**Exception — the platform layer.** `Source/Platform/**` holds plain ObjC++ (`.mm`) / C++ (`.cpp`) translation units, because CMake's module scanner does not scan `OBJCXX` sources and they therefore cannot be modules. Each exposes a narrow plain-C++ header that a `jpt.<Name>` module wraps; keep the ObjC types behind an opaque handle or pimpl so they never appear in the header. This is the only place standalone TUs belong.

### Module attachment across the platform boundary

An entity's **module attachment is fixed by its first declaration, and every definition must match it.** Clang enforces this at compile time, and it constrains the platform seam in both directions:

- A function declared in a plain header (global module) **cannot be defined inside a module purview** — `error: declaration of 'X' in module jpt.Foo follows declaration in the global module`. This is why the platform→app callbacks in `Window/Apple/AppleCallbacks.h` are defined in their own plain `.cpp` that does `import jpt.Application;`, not in `Application.cpp`.
- Conversely an `export`ed function **can only be defined by its own module**, so a client cannot supply it. `jpt::GetApplication()` therefore lives in the plain `Applications/AppClient.h`, letting `Main.cpp` own the instance (the clang-compatible form of legacy's `JPT_SYNC_CLIENT`).

Note MSVC is more permissive here; legacy Jupiter code that compiled on Windows may not compile on clang.

### Headers and `import`

Clang accepts `import` inside a global module fragment, so a plain header may `import jpt.TypeDefs;` and use `uint32`/`float64` even when included from a `.cppm`. Two rules bound that:

**Hard constraint: `.mm` files must never `import std;`, directly or transitively** — Clang refuses to load a C++-built `std.pcm` into an Objective-C++ translation unit (`error: Objective-C was disabled in precompiled file 'std.pcm' but is currently enabled`). Since `jpt.TypeDefs` imports std, **any header a `.mm` includes must be import-free** and spell types as `std::uint32_t` / `double`. That currently covers `AppleCallbacks.h`, `MacWindow.h`, and `IOSWindow.h`. Headers never included by a `.mm` (`Window.h`, `Renderer.h`) use the `jpt` aliases.

**Include textual headers before headers that import.** A TU that mixes `import std` with textual libc++ headers must see the textual copies first, or the module's declarations win and the later includes are a redefinition: `error: type alias template redefinition with different types` in `__promote.h`. See the ordering comment at the top of `Graphics/Metal/MetalRenderer.cpp`.

## Build flow

The build is driven by CMake + Ninja + clang++ (set unconditionally in `CMakePresets.json`). Three thin Python scripts in `Scripts/` wrap the CMake commands so the same workflow works on every host:

```
py Scripts/setup.py [debug|dev|release]   # configure (writes _ProjectFiles/setup.json + .vscode/launch.json)
py Scripts/build.py                       # build the configured preset
py Scripts/run.py                         # run _Output/<preset>/JupiterEngine
py Scripts/clean.py                       # remove _Output, _ProjectFiles, .vscode/launch.json
```

`setup.py` picks the platform automatically (`windows-*`, `macos-*`, `linux-*`) and prompts for the config if none is passed. `build.py` and `run.py` read `_ProjectFiles/setup.json` to know which preset to use — both fail with "Run: py Scripts/setup.py" if it is missing. All scripts resolve paths relative to the repo root (`Path(__file__).resolve().parent.parent`), not the caller's working directory.

The same four steps are exposed as VS Code tasks (`Setup`, `Build`, `Run`, `Clean`) in `.vscode/tasks.json`. `.vscode/launch.json` is **generated by `setup.py`** to point at the active preset's binary — do not hand-edit it; if F5 launches the wrong path, re-run `Setup` (or fix the generator in `setup.py`) instead. `clean.py` deletes it.

## Layout that matters

- `Source/Main.cpp` — the entry point; imports and composes engine modules. Add new engine code as a module (see below) or in headers `Main.cpp` includes directly; do not add new `add_executable` targets or standalone `.cpp` translation units.
- `Source/**/*.cppm` — engine subsystem modules (e.g. `Source/Debugging/Logger.cppm` → `export module jpt.Logger;`). `CMakeLists.txt` discovers every `.cppm` under `Source/` automatically via `file(GLOB_RECURSE ... CONFIGURE_DEPENDS "Source/*.cppm")`, so adding a module file needs no `CMakeLists.txt` edit. Folder placement is a loose grouping only — the module name is `jpt.<Name>`, not tied to the folder path.
- `CMakeLists.txt` — defines platform macros (`IS_PLATFORM_WINDOWS|MACOS|LINUX|ANDROID|IOS`) and config macros (`IS_CONFIG_DEBUG|DEV|RELEASE`), each always defined as `0`/`1` so code uses `#if IS_PLATFORM_WINDOWS` directly rather than `#ifdef`/`#if defined(...)`. Use these instead of raw `_WIN32`/`__APPLE__` checks. Final binary lands in `_Output/${JUPITER_PRESET}/`, which is set from the active preset name.
- `CMakePresets.json` — `*-debug` / `*-dev` (RelWithDebInfo + `-O3 -g -DNDEBUG`) / `*-release` per OS. The "dev" config is the optimized-with-symbols variant the scripts default to.
- `_ProjectFiles/` — CMake intermediates **and** `setup.json` (the active-preset record the scripts read). Gitignored. Wiped by `clean.py`.
- `_Output/<preset>/` — final binary directory the run/launch configs target. Gitignored.

## Conventions

- C++23, no compiler extensions (`CMAKE_CXX_EXTENSIONS OFF`).
- Compiler is pinned to `clang++` across all hosts via the base preset — assume clang diagnostics/flags, not MSVC.
- On Apple platforms the compiler must be **Homebrew LLVM** (`/opt/homebrew/opt/llvm/bin/clang++`), for both `CXX` and `OBJCXX`. Apple clang cannot build C++ modules at all: Xcode ships no `clang-scan-deps`, so CMake fails with *"the compiler does not provide a way to discover the import graph dependencies"*, and it has no `libc++.modules.json` for `import std`. Homebrew clang cross-compiles ObjC++ against the Apple SDKs and links the **system** `/usr/lib/libc++.1.dylib`, so nothing needs embedding in an iOS bundle.
- Shell-based scripts have been deliberately replaced with cross-platform Python (commit `827a515`); keep new automation in Python and route it through the existing `setup → build → run` scripts rather than adding `.sh`/`.bat`.

### Python style (`Scripts/`)

- **Docstrings, per [PEP 257](https://peps.python.org/pep-0257/).** Document a function with a `"""..."""` as the first statement in its body — one line for simple cases, or a summary line plus a blank line and detail. Do not use banner comments above `def`; they leave `__doc__` as `None`, so doc generators and any programmatic introspection see nothing.
- **Annotate return types.** Every function that returns a value ends its signature with `-> <type>` (`-> str`, `-> bool`, `-> Path`, `-> int`, `-> list | None`). Functions that return nothing take no annotation.
- Shared constants and path helpers live in `Scripts/utils.py` so `setup.py` / `build.py` / `run.py` / `Clean.py` never duplicate a path or a name. `artifact_path()` is the single place that knows a macOS binary lives inside `.app/Contents/MacOS/` while an iOS `.app` is installed whole.
- Scripts take arguments only; they never prompt interactively. On bad or missing input, print `utils.USAGE` and exit non-zero.
