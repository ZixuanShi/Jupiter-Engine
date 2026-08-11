# SDL3

Window, input and entry point on every platform. The renderer is not SDL's: `SDL_Metal_GetLayer`
hands back the real `CAMetalLayer` that `RendererMetal4` configures, and `SDL_Vulkan_CreateSurface`
will do the same for Vulkan.

- Source: https://github.com/libsdl-org/SDL
- Branch: `release-3.2.x`
- Vendored commit: `402fc52af4e731184ad6a704068b5ccd27d8f1b8` — SDL 3.2.31 (2026-05-27)

**`Source/` is committed, not restored by hand.** `git clone` then `setup → build` is the whole
onboarding; see `.gitattributes`, which stores these bytes as-is so Git for Windows does not
rewrite 1500 LF files to CRLF, and keeps them out of diffs. A pinned SHA in a README is a promise,
a committed tree is a fact — and the promise was already broken once, on the second machine.

Only the build tree is vendored — `test/`, `examples/`, `Xcode/`, `VisualC*/` and `android-project/`
are dropped, which is 30 MB instead of 60. To bump the version, replace those directories from a
fresh clone of the new tag and commit the result:

```
git clone --depth 1 --branch release-3.2.x https://github.com/libsdl-org/SDL.git /tmp/sdl3
cp /tmp/sdl3/{CMakeLists.txt,LICENSE.txt} Vendor/SDL3/Source/
cp -R /tmp/sdl3/{src,include,cmake,build-scripts,wayland-protocols} Vendor/SDL3/Source/
```

## Notes

Built through SDL's own CMake with `add_subdirectory`, unlike Dear ImGui whose sources are listed
by hand: SDL is ~700 files with real platform detection, so its CMake is the interface.

**Static, never shared.** `Scripts/Build/sign.py` runs `codesign` without `--deep`, so a dylib
nested in the bundle would ship unsigned, and `Projects/<Name>/CMakeLists.txt` gives iOS no rpath
to find one with.

## Measured on 2026-08-10, SDL 3.2.31, Windows 11 + llvm-mingw

Nothing needed adding for Windows. The one SDL block in `CMakeLists.txt` is outside every
conditional and stays that way: SDL's own CMake found the Win32 backends, `SDL3::SDL3-static`
carried its system libraries in through its interface, and `SDL_MAIN_USE_CALLBACKS` produced a
console-subsystem `main` that `Scripts/run.py` launches with stdout attached.

**`SDL_HINT_TRACKPAD_IS_TOUCH_ONLY` is set unconditionally and costs nothing here.** It exists in
the Windows headers and only the Cocoa backend reads it, so the alternative — a `#if` in
`Window::PreInit` — would buy a guard and no behaviour.

**Windows reports `pixel density 1.00`, with window points and backing pixels equal.** Measured at
1920x1080: SDL has no "points" on Win32, so the reconciliation assert in `Window::Init` holds by
construction rather than by luck. `SDL_EVENT_WINDOW_PIXEL_SIZE_CHANGED` does fire — a 1024x768
window reported `1008x729`, its client area — so `OnResize` is live even with a null renderer.

**Nothing throttles the frame loop off Apple.** `SDL_AppIterate` is called as fast as it returns,
and `RendererNull::BeginFrame()` declines without ever presenting a drawable, so `Blank.exe` spins
a full core. On Apple the Metal present blocks on the display link. Frame pacing is Vulkan's to
bring, not `RendererNull`'s to fake.

## Measured on 2026-08-10, SDL 3.2.31, macOS 26

Read from `src/video/cocoa/` and `src/events/`, not from the wiki.

**`SDL_HINT_TRACKPAD_IS_TOUCH_ONLY` must be set to `"1"` on macOS, before `SDL_Init`, or there are
no trackpad gestures at all.** Cocoa reports a trackpad as the *mouse's* touch device —
`SDL_cocoawindow.m`'s `isTouchFromTrackpad:` returns YES, so `handleTouches:` picks
`SDL_MOUSE_TOUCHID` — and `SDL_SendTouch` then drops it on the floor:

```c
// SDL_touch.c:316
if (!mouse->mouse_touch_events && (id == SDL_MOUSE_TOUCHID)) { return; }
```

`mouse_touch_events` defaults to true only on Android and iOS (`SDL_mouse.c:169-173`). So on macOS
`handleTouches:` runs, `SDL_AddTouch` succeeds, and every send returns before reaching the queue:
**zero `SDL_EVENT_FINGER_*`**. The hint makes the trackpad report under its real device id instead,
which the guard never touches.

*An earlier note here claimed the default delivered both mouse and fingers. It does not — that
reading stopped at `handleTouches:` and missed the discard one layer down.*

**The mouse survives the hint.** `trackpad_is_touch_only` has exactly three uses, all in the touch
classification path; `mouseMoved:` and `scrollWheel:` never consult it. So the hint gains fingers
and costs nothing.

**Not `SDL_HINT_MOUSE_TOUCH_EVENTS` instead.** It registers `SDL_MOUSE_TOUCHID` as a *direct*
device (`SDL_mouse.c:176`) and `SDL_AddTouch` returns early on a duplicate id (`SDL_touch.c:172`),
so the backend's later `INDIRECT_ABSOLUTE` is silently ignored and the engine's coordinate branch
takes the wrong axis. It also synthesizes a finger from every mouse motion.

**Trackpad touches are `SDL_TOUCH_DEVICE_INDIRECT_ABSOLUTE`**, not `INDIRECT_RELATIVE`.
Coordinates are `[touch normalizedPosition]` with y flipped to a top-left origin, and `fingerID` is
`[touch identity]` — so SDL does the id-minting and normalization the old backend hand-rolled with
an `NSMutableDictionary`.

**SDL exposes no trackpad `deviceSize`, and that is a real loss.** `normalizedPosition` is
normalized per axis over the pad's own extents, so it is anisotropic; the deleted AppKit code undid
that with `device.width/device.height`. `Window.cpp` stands in a measured constant — Apple
trackpads are ~1.6:1 — because nothing in SDL can recover the true ratio. Without it a physical 45°
twist reads about 32°.

**`isPrecise` from a fractional delta is sound.** `Cocoa_HandleMouseWheel` ceils/floors the delta
when `hasPreciseScrollingDeltas` is false and passes it through raw when true, so fractional ⇔
precise. `wheel.which` is always `SDL_DEFAULT_MOUSE_ID` and is not a discriminator.

**Toolchain:** Homebrew clang (`/opt/homebrew/opt/llvm/bin/clang`) builds SDL's C and Objective-C
for macOS and cross-builds `SDL3-static` for `iphoneos`/arm64 at deployment target 26.0. No Apple
clang fallback is needed.

**Build cost, measured:** a clean configure is ~33 s, almost all of it SDL's own ~360 CMake platform
probes; the compile that follows is ~24 s. Turning off the unused subsystems (audio, joystick,
haptic, hidapi, sensor, camera, power, dialog, render, gpu) was tried and **reverted**: it saved
0.8 s of configure and 28 of 211 objects, because `video` (172 sources) dominates and SDL stubs each
disabled subsystem anyway. `Scripts/clean.py` wipes `_ProjectFiles`, where SDL builds, so a clean
rebuild pays both costs every time.
