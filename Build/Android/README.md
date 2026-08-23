# Android

`Template/` is the gradle project `setup.py` copies into `Projects/<Name>/_ProjectFiles/android/`
with the name tokens filled in. **Gradle never sees native code**: the CMake presets build
`libmain.so` with CMake 4.2.3 + Ninja (AGP's bundled CMake 3.22 cannot build C++ modules) and write
it straight into the copy's `jniLibs/`, `stage_assets.py` fills its `assets/`, and gradle does
javac, the manifest, debug signing and 16 KB alignment. Edit the template, never the copy — setup
re-instantiates it, but only ever copies *over*, so the built `.so` and staged assets survive.

Pinned: gradle 8.9 (wrapper committed; first build downloads the distribution once), AGP 8.7.3,
minSdk 29, targetSdk/compileSdk 35, `arm64-v8a` only, debug signing for every preset — the native
debug/dev/release config is baked into `libmain.so`, so a release APK would only add a signing
config nobody has. SDL is linked statically into `libmain.so`, which is why `JupiterActivity`
narrows `getLibraries()` to `{"main"}`; the SDLActivity it extends is vendored at
`Vendor/SDL3/Android`, same commit as SDL's C — see that README.

The toolchain story — the NDK ships no std module, and what stands in for it — lives in
`Vendor/LibCxxModules/README.md`.

## Wireless adb (the OnePlus USB driver never enumerates)

- Once: phone Settings → Developer options → Wireless debugging → *Pair device with pairing code*,
  then `adb pair <ip:pairing-port>` with the code. Pairing survives reboots.
- Each session: `adb connect <ip:port>` with the port Wireless debugging currently shows — it
  changes per session. `adb devices` should then list the phone as `device`.
- `py Scripts/run.py` takes it from there: `adb install -r`, `am start -W`, then a logcat scoped
  to the app's pid until Ctrl+C detaches (the app keeps running).

## Measured on 2026-08-22, OnePlus CPH2583 (ColorOS 15), Blank + RendererNull

**ColorOS drops third-party *native* logs by default.** Everything written through
`__android_log_write` — every `Debug::Log` in the engine — is discarded before logd, while the
same app's *java* logs show once `adb shell setprop log.tag.SDL V` allows the level, and shell-uid
`log` works. `persist.sys.assert.panic` cannot be set over adb on an unrooted phone; the switch is
on the device: dial `*#800#` → logkit → *save log* (spelling varies by ColorOS version). Until it
is flipped, verify the engine by its behavior, not its banner — everything below was.

- Launch: `nativeloader` reports `Load .../libmain.so ... : ok`, SDLActivity runs
  `onCreate → nativeSetupJNI → surfaceCreated`, surface 1080x2376, `SDLThread` appears.
- **The unthrottled frame loop pins one core** (~100% in `top`), same as Windows — RendererNull
  never presents, so nothing paces `SDL_AppIterate`. Expected until Vulkan; the phone warms.
- A running process is itself the Init verdict: a failed `Init()` (a `LoadObj` miss included)
  returns `SDL_APP_FAILURE` and SDLActivity tears the process down within seconds. Minutes at
  100% mean window, surface, input and the APK asset read all came up.
- Home gesture: process survives at **0.0% CPU** — `Status::Paused` parks `OnFrame` and SDL stops
  iterating a backgrounded app. Returning resumes the same pid at 100%.
- **The back gesture does not quit.** SDL delivers it as `SDL_SCANCODE_AC_BACK` and the activity
  stays resumed; nothing in the engine binds it. Quitting an SDL Android app is the recents
  swipe-kill — SIGKILL, so `Terminate()` does not run and no farewell line is owed, same as iOS.
- Touch swipes deliver through the finger→gesture pipeline without incident (no crash, no
  tombstone); the camera they steer draws nothing yet.
