# CLAUDE.md

Guidance for Claude Code when working in this repository.

## Measure it, test it. Don't guess.

Verify by compiling and running a probe, not by reasoning about the spec. Skipping this has twice
produced a confident wrong answer here — a winding setting "justified" by a Y-flip that does not
apply, and a `LookAt` bug diagnosed as NaN when `Vector3::Normalize()` already guards its divide.

- **A test that cannot fail is not a test.** After fixing a bug, revert the fix and confirm the
  assertion fires.
- Assert what breaks, not a proxy. That `LookAt` kept a correct translation column while its basis
  lost rank, so only an orthonormality check caught it.
- Say what was verified here versus what is documented-but-unrun.

## Project intent

A cross-platform game engine. **SDL3** is the window, input and entry point on every platform;
the renderer is **Metal 4** on macOS/iOS and **Vulkan** on Windows/Linux/Android.
`CMakePresets.json` wires up `macos`, `ios-device`, `windows`, `linux`, `android`.
**Windows opens a real window today and draws nothing** — verified end to end on 2026-08-10:
lifecycle, resize and keyboard/mouse into `jpt::Input`, closing cleanly once. The renderer seam
answers `RendererNull` there. **Android reached the same parity on 2026-08-22**, verified on a
real phone: `libmain.so` in a debug APK, surface, touch into `jpt::Input`, pause/resume; the
wireless-adb flow and every measured device behavior live in `Build/Android/README.md`, the
std-module story in `Vendor/LibCxxModules/README.md`. **Linux does not build at all**,
`PlatformPaths.cpp` being an `#error` for it; the preset exists, the platform does not.
See Platform backends.

**Engine and app are separate targets.** `Source/**` builds the static library
`_Output/<preset>/libJupiterEngine.a`; the executable comes from the active project under
`Projects/<Name>/`, into that project's own `_Output/`, `_ProjectFiles/` and `_Saved/`. One project
per build tree — the root `CMakeLists.txt` ends with `add_subdirectory(${JUPITER_PROJECT})`, and
`setup.py` chooses it.

`Source/Main.cpp` stays engine-owned but is compiled into the app target, not the archive:
`JUPITER_MAIN_SOURCE` is removed from the engine glob and named again by the project. Keeping it in
the archive would leave the entry point to whether the linker resolves `main` from a member.

Assume one executable *per project*. Everything app-shaped — the bundle, the plist, asset staging,
`default.metallib`, signing, rpath — lives in `Projects/<Name>/CMakeLists.txt`, because all of it
names the app target. It is deliberately written out rather than hidden behind a `jpt_add_app()`
helper: with one project there is no duplication to factor, and `MACOSX_BUNDLE_*` are read from the
target's *directory* scope, so a `function()` would silently emit an empty Info.plist.

## Build

```
py Scripts/setup.py <debug|dev|release> <macos|ios-device|windows|linux|android> [project]
py Scripts/build.py                                                        # builds the configured preset
py Scripts/run.py
py Scripts/clean.py                                                        # engine + every Projects/* underscore dir
```

`setup.py` takes the project as a bare name resolved under `Projects/` (`Blank`) or as a path, and
records it in `_ProjectFiles/setup.json` — which stays at the repo root, being engine-level state.
**It is required, never inferred**, and `JUPITER_PROJECT` has no CMake default either: which project
you are building decides the binary, the bundle id and where `_Output` goes, so guessing one would
only produce a build nobody asked for. The binary name is the project directory's name and the
bundle id is `com.jupitertechnologies.<name lowercased>`; `utils.py` derives both, and
`Projects/<Name>/CMakeLists.txt` spells the same convention for CMake.

There are two projects — `Blank` and `UnitTests` — and **only one is configured at a time**, since
`setup.json` and the generated `.vscode/launch.json` each name exactly one. Switching between them
is a re-run of Setup, not a build flag.

The preset's `binaryDir` reads `$env{JUPITER_PROJECT_DIR}`, which `setup.py` and `build.py` export.
A hand-run `cmake --preset` without it resolves to a bogus path, so go through the scripts. CMake
itself reads the `JUPITER_PROJECT` cache variable, so a ninja-triggered reconfigure needs no
environment.

Those four are the whole command surface, so they are the only scripts in `Scripts/`. Everything
else is a subfolder, invoked by CMake or by hand rather than as a step you run:

| | |
|---|---|
| `Scripts/utils.py` | shared constants and path helpers — a library, not a command, so it stays at the import root |
| `Scripts/Build/` | `sign.py` (CMake, iOS), `provision.py`, `generate_std_module.py` (NDK bumps) |
| `Scripts/Assets/` | `stage_assets.py` (CMake), `normalize_mesh.py` |
| `Scripts/Debug/` | `pull_captures.py` |

A subfolder script is still run by path, so its `sys.path[0]` is its own directory rather than
`Scripts/`. Each one therefore opens with `sys.path.insert(0, ...parents[1])` before
`from utils import ...` — without it the import fails at run time, which for `sign.py` means a
failed build rather than a failed edit.

`build.py`/`run.py` read `_ProjectFiles/setup.json` for the active preset. All scripts resolve paths
from the repo root, not the caller's cwd. The same four steps are VS Code tasks;
`.vscode/launch.json` is **generated by `setup.py`** — re-run Setup rather than hand-editing it.

Final binary lands in `_Output/<preset>/`. Both `_ProjectFiles/` and `_Output/` are gitignored.

An android build is the same three commands with one more stage inside them: `setup.py`
instantiates `Build/Android/Template` into the project's `_ProjectFiles/android/`, the CMake
build links `libmain.so` (a `SHARED` library — `SDL_MAIN_EXPORTED` leaves no native `main` to
build an executable around) into its `jniLibs/`, and `build.py` then runs gradle, which only
packages: the artifact is `_Output/<preset>/<Name>.apk`, and `run.py` drives it over adb.

`CMakeLists.txt` defines `IS_PLATFORM_WINDOWS|MACOS|LINUX|ANDROID|IOS`,
`IS_CONFIG_DEBUG|DEV|RELEASE` and `IS_EDITOR` as `0`/`1` always, so use `#if IS_PLATFORM_MACOS` —
never raw `_WIN32`/`__APPLE__`, never `#ifdef`.

**`IS_EDITOR` is not `!IS_CONFIG_RELEASE`.** It means the editor *and the Dear ImGui it draws
through* are compiled in, which a dev build on a platform with no ImGui backend is not — today
that is every non-Apple dev build. It gates `Source/Editor/` out of the glob, the `JupiterImGui`
target, `ApplicationBase::m_editorUI`, and which half of `ImGuiLayer` defines the layer: the `.mm`
holds the implementation, `ImGuiLayer.cpp` the no-ops that let every caller stay unguarded. Those
no-ops must live in a plain `.cpp`: as the `#else` of the `.mm` they were unreachable on a platform
that compiles no Objective-C++, which is a link error rather than a compile one.

## Modules

Engine subsystems are C++23 modules: `export module jpt.<Name>;`, contents in `namespace jpt`.
`Main.cpp` is the single non-module entry point. Every `.cppm` under `Source/` is globbed
automatically, so adding one needs no CMake edit; folder placement is loose grouping only. A
project's `Source/` is globbed the same way into its own target.

**A module name is not a filename, and it cannot be macro-expanded.** That is why every project
exports `jpt.App` whatever it calls the file or the class — a fixed name is the only way
engine-owned `Main.cpp` can `import` a type the engine cannot name. See Lifecycle.

A subsystem may split into the interface unit `Foo.cppm` and an implementation unit `Foo.cpp`
(`module jpt.Foo;`, no `export`). The CMake file set is for **interface units only** — implementation
units are ordinary sources, or the link fails with undefined `jpt::Foo@jpt.Foo::Bar()`.

These rules each cost a build cycle when forgotten:

- **Every TU needs its own global module fragment.** A GMF is private to its file. An implementation
  unit's `#include`s belong between `module;` and `module jpt.Foo;`, or their declarations attach to
  `jpt.Foo` and clang rejects them as redeclarations of the global module. A plain non-module `.cpp`
  has no module declaration and escapes this.
- **An `import` is not re-exported.** `import jpt.ApplicationBase` gives you `ApplicationBase`, but
  `app.GetCamera().GetViewProjection(...)` still needs `import jpt.Camera;` of its own.
- **Imports must be contiguous** directly under the module declaration — an `#include` in the middle
  gives *"imports must immediately follow the module declaration"*.
- **`.mm` files must never `import std;`, directly or transitively.** Clang refuses a C++-built
  `std.pcm` in an ObjC++ TU. Since `jpt.TypeDefs` imports std, **any header a `.mm` includes must be
  import-free** and spell types `std::uint32_t`/`double` — today that is `ImGuiLayer.h` alone, since
  `ImGuiLayer.mm` is the last `.mm`. Everything no `.mm` includes may use the `jpt` aliases, which
  is what let the window headers become `jpt.Window`.
- **Include textual headers before headers that import**, or the module's declarations win and the
  later include is a redefinition (`__promote.h`). See the ordering comment in `RendererMetal4.cpp`.
- **Module attachment is fixed by an entity's first declaration.** A function declared in a plain
  header cannot be defined in a module purview (why `ImGuiLayer.h`'s functions live in a plain
  `.mm`/`.cpp` pair); an `export`ed one can only be defined by its own module. This is also what keeps a
  project's `Application<Name>.cpp` a plain TU — see Lifecycle. MSVC is more permissive — legacy
  code that built on Windows may not build on clang.

**Exception — the platform layer.** `Source/Platform/**` and the Apple window code are plain
`.mm`/`.cpp`, because CMake's scanner does not scan `OBJCXX`. Each exposes a narrow plain-C++ header
that a module wraps; keep ObjC types behind a pimpl. This is the only place standalone TUs belong.

## Platform backends

**One seam is left**, and it is the renderer: `Graphics/Renderer.h` picks the `Renderer` by
`#if IS_PLATFORM_*`, checked against the `RendererType` concept and a
`static_assert(!std::is_polymorphic_v<...>)` — compile-time polymorphism, so a backend *hides* the
base's function rather than overriding it.

**The window seam is gone.** SDL3 is every platform's window, input and entry point, so there is
one `jpt.Window` module and no base, no concept and no alias — a seam with one implementation only
checks a type against itself. Only `Window::SurfaceHandle` is conditional: `CA::MetalLayer*` on
Apple, `void*` elsewhere until a Vulkan surface exists. `ApplicationBase::Init()` takes the surface
from the window and hands it to the renderer; `Source/Debug/StaticAsserts.cpp` is what holds the
two independently chosen types to the same one. A console SDL does not cover is when the concept
and the alias come back — public SDL3 has Xbox (GDK), PS2/PSP/Vita/3DS, while PS5 and Switch are
NDA-gated private ports.

Being a module rather than a plain header is what lets `Window::OnEvent` take a real
`const SDL_Event&`: SDL's headers sit in the module's global module fragment, which importers never
see. That was impossible while a `.mm` included the window header — none does now, `ImGuiLayer.mm`
being the only Objective-C++ translation unit left.

Off Apple the renderer seam still answers `RendererNull` (`Graphics/Null/`), **not** an `#error`.
`ApplicationBase` holds it by value, so an `#error` there is not a missing backend, it is an engine
that will not compile at all — and Windows/Linux/Android then cannot run so much as a log line.
Nothing gives it a surface, so `RendererNull::BeginFrame()` declines every frame. It is what
**Vulkan** replaces, not extends.

That has one measured consequence worth knowing before it is reported as a bug: **the frame loop is
unthrottled there and spins a full core.** `SDL_AppIterate` is called as fast as it returns, and
what paces a frame on Apple is the Metal present blocking on the display link — which a renderer
that never presents cannot do. Frame pacing arrives with Vulkan; do not fake it in `RendererNull`.

The same shape is what off-Apple `LoadTexture` still owes: it is declared unconditionally and
returns an empty `Texture` with an error, pending stb_image or WIC.

## Metal 4

The renderer is `Source/Graphics/Metal/RendererMetal4.{h,cpp}` — Metal 4 (`MTL4::CommandQueue`,
`CommandAllocator`, `ArgumentTable`, `Compiler`), not classic Metal. Hard floor of **macOS 26 /
iOS 26**, hence `CMAKE_OSX_DEPLOYMENT_TARGET 26.0`.

**There is deliberately no `ios-sim` preset.** The Simulator SDK ships the `MTL4*.h` headers but
declares none of the protocols in them, and its runtime exports zero MTL4 symbols. Test on a device,
which is the better test anyway — the Simulator never modelled TBDR, tile memory, or memoryless depth.

- **`setVertexBytes` does not exist.** Everything a shader reads is a GPU address in an
  `MTL4::ArgumentTable`, so per-frame constants need a real buffer — hence the ring of
  `kFramesInFlight` (3) uniform slots and allocators behind a `std::counting_semaphore` released
  from the commit feedback handler. ImGui gets the same count and slot index.
- **The argument table is per-stage.** A stage with no table bound reads an undefined address.
- **Residency is manual.** Anything reached by raw address must be in an `MTL::ResidencySet`; a miss
  is a GPU fault, not a validation error. Memoryless textures must **not** be added (the layer
  asserts); render-pass attachments don't need it.
- **The pipeline has no depth attachment format** — depth comes from the render pass at encode time.
- `Graphics/Shader/ShaderTypes.h` is compiled by clang **and** `metal`, so it stays import-free with
  `static_assert(sizeof(...))` checked on both sides. Never put a 3-component vector in it: MSL's
  `float3` is 16 bytes, a plain `float x,y,z` is 12, and nothing warns.

Run with `MTL_DEBUG_LAYER=1` when changing the renderer.

## Coordinate system

**Right-handed, +X right, +Y up, −Z forward**, because a right-handed basis is `X × Y = Z`.

**"+X right, +Y up, +Z forward" is left-handed and was considered and rejected** — an object facing
+Z has its right hand toward −X. That ask comes from wanting an intuitive editor, which this already
gives: the basis is self-consistent for objects, so a gizmo is
`rotation * Vec3::Right()/Up()/Forward()` and the user never sees a sign. Axis labels are UI.

RH is kept because glTF 2.0, Maya and USD are RH/+Y-up, so meshes import with no conversion — and a
handedness conversion is *not* "negate Z", it also flips winding and mirrors normals and tangents.

**Two "forwards".** *View forward* is −Z, what `LookAt`/`Perspective` assume. *Model front* is
however the mesh was authored, and **glTF authors assets facing local +Z** — import one, rotate
nothing, and it faces away from `Forward()`. Rule not yet chosen; record it here when it comes up.

**Clip space** is `z ∈ [0, 1]`, which Metal and Vulkan agree on; both projections are verified
near→0.0, far→1.0. Metal's NDC is **+Y up**, Vulkan's is **+Y down**, so the Vulkan backend applies
the flip — preferably a negative viewport height, leaving the matrix identical across backends.

**Any Y flip inverts triangle orientation, so front-face winding must flip with it.** Metal is
verified `WindingCounterClockwise` for the CCW-from-outside geometry the OBJ loader produces. Do not
port that value to another backend. Getting it wrong does not look like a culling bug — it looks like
an inverted camera, because you see the model's interior.

## Lifecycle

Every subsystem exposes the same five, driven by `ApplicationBase` in this order. Implement only
what is needed; no empty overrides.

| | When | May touch |
|---|---|---|
| `PreInit()` | once, first | **itself only** |
| `Init()` | once, after every subsystem pre-initialised | itself **and** others |
| `Update()` | every frame | anything |
| `PostUpdate()` | every frame, after every `Update()` | anything |
| `Terminate()` | once, on exit | anything |

The split is the point: `PreInit` may not reach another subsystem because pre-init order is not a
contract. `PostUpdate` is for work that must observe every `Update`.

`PreInit`/`Init` return `bool` and abort startup on false; the rest return `void`. All are
parameterless — SDL owns the entry point, so nothing is handed `argc`/`argv`.

**There is no `Run()`.** `Main.cpp` hands SDL four callbacks and never blocks:
`SDL_AppInit` → `PreInit()` + `Init()`, `SDL_AppIterate` → `OnFrame()`, `SDL_AppEvent` →
`Window::OnEvent()`, `SDL_AppQuit` → `Terminate()`. On iOS `SDL_AppIterate` *is* the display-link
animation callback, which is why frame pacing survived the move off `CADisplayLink`.

**`m_status` is the lifecycle, and `SetStatus(Status)` is the only way it moves.** `SDL_AppIterate`
and `SDL_AppEvent` both return `local::ToAppResult(GetApp().GetStatus())`, so SDL asks the app for a
verdict instead of being handed one: `Running`/`Paused`/`Pending` continue, `Failed` exits 1,
anything else exits 0. `Window::OnEvent` returns `void` and reports close requests and iOS
backgrounding through `SetStatus`, so one field decides the run and there is no second authority to
disagree with it. **Pausing must map to CONTINUE** — returning SUCCESS for a backgrounded iOS app
would quit it.

`SetStatus` writes only while the app is `Running` or `Paused`, and that single guard is why
pausing and quitting are not two functions. It makes a quit first-request-wins — a window close
arriving after a failure cannot downgrade the exit code — and by the same test stops a foreground
event arriving after `SDL_AppQuit` resurrecting a terminated app. Startup does not end through it:
`PreInit`/`Init` abort by returning false, and `Init()` finishes by setting `Running`, so anything
asked for before that would be clobbered.

**`Terminate()` has no re-entry guard, and must not grow one back.** It carried an
`if (m_status == Status::Succeeded) return;` for a long time, which is a bug the moment `m_status`
becomes the lifecycle: the status goes terminal when the quit is *requested*, teardown runs later,
so the guard sees the verdict already there and returns — the renderer and window are never
released, and nothing looks wrong because SDL latched the exit code before calling `SDL_AppQuit`.
Measured. Replacing it with a separate `m_terminated` flag works, but measurement said the guard
has nothing to do: `SDL_AppQuit` fires exactly once per process on every backend — including after
a failed `SDL_AppInit`, and iOS's two `SDL_QuitMainCallbacks` sites are mutually exclusive and both
`exit()` — and calling `Terminate()` twice by hand is harmless anyway, since every subsystem nulls
its own handles and `ImGuiTerminate()` returns early on a null context. The only symptom was a
duplicated banner. `m_status` therefore survives the run as its outcome, and nothing shadows it.

**Subsystems pull, they do not get pushed.** `Update()` fetches what it needs via `GetApp()`
(`Applications/GetApp.h`) — see `RendererBase::Update()`. `ApplicationBase::Update()` stays a list
of calls, and ordering stays explicit because it still decides the call order. **Only an
implementation unit or a plain `.cpp` may do this**: `jpt.ApplicationBase` imports the subsystem
modules, so `App.h` in a `.cppm` gives *"Circular dependency detected in the C++ module import
graph"*.

### `ApplicationBase` is the only virtual, and that exception is load-bearing

`static_assert(!std::is_polymorphic_v<...>)` in `Graphics/Renderer.h` keeps
everything *below* it vtable-free: a backend *hides* the base's function and must call it explicitly
by name, `RendererBase::PreInit()`. Nothing enforces that; a backend behaving as if its base state
was never set is the symptom.

`ApplicationBase` is deliberately different. Its five lifecycle functions, its destructor and
`OnSurfaceReady` are `virtual`, and a project writes `override`.

**Devirtualizing it was tried and reverted.** Without `virtual`, engine code has to name the
project's type to reach it, and a static library cannot -- importing the app target's BMI is a CMake
cycle. Working around that took a macro of link-time thunks, a concept to catch the mistyped
"overrides" that hiding makes silent, a second accessor, and splitting `OnFrame` into halves so the
base never called down. All of it bought one vtable pointer in a single process-lifetime object and
about four indirect calls per frame. Keep the exception.

What it buys instead is the whole engine/app seam in one declaration plus one macro,
`Applications/GetApp.h`:

```cpp
namespace jpt { ApplicationBase& GetApp(); }   // engine declares

#define JPT_SYNC_APP(AppClass) ...                     // project defines, once
```

Engine code calls through `ApplicationBase&` and lands on the project's overrides, so nothing in
`libJupiterEngine.a` names an app-target type. `Main.cpp`'s `SDL_AppIterate` calls
`GetApp().OnFrame()`, and `ApplicationBase::OnFrame()` calls its own `Update()` /
`PostUpdate()` and gets the project's.

A project is two files in `Projects/<Name>/Source/`: `Application<Name>.h` declaring
`class Application<Name> final : public ApplicationBase` with its `override`s, and
`Application<Name>.cpp` holding the bodies and, at the bottom, `JPT_SYNC_APP(Application<Name>)`.

**`jpt::GetAppName()` cannot go through this seam**, and that is what makes it the seam's edge.
It is `consteval`, and an immediate function must be *defined* in every TU that calls it: constant
evaluation happens in the compiler, and the linker — the whole mechanism `GetApp()` relies on —
has nothing to contribute to it. So the name arrives as `JUPITER_APP_NAME`, which the root
`CMakeLists.txt` derives from the project directory, and the body lives in `GetApp.h`. Anything
the engine needs from a project at *compile* time has to come the same way.

**Both are plain, not a module, and that is forced.** `GetApp()` is first declared in a
plain header, so it is attached to the global module; expanding the macro in a module unit is
*"declaration of 'GetApp' in module jpt.X follows declaration in the global module"* —
measured, not assumed. It costs nothing: this is a leaf class with one consumer, and it is the same
shape as `RendererMetal4.h`, a plain header whose class derives from the `jpt.RendererBase` module.
Anything *else* a project adds may be a `.cppm`; the glob already picks them up.

`Projects/Blank` is the worked example. `Projects/UnitTests` is the second, and the one place a
project ends its own run: `ApplicationUnitTests::Init()` calls `SetStatus` with the runner's
verdict — see Lifecycle. It needs no SDL include to do it.

**The suites live there, not in the engine**, and nothing engine-side calls a test any more, which
is why the root `CMakeLists.txt` no longer strips `*Tests` in Release.

### Nothing registers itself; one file lists the run

`Projects/UnitTests/Source/TestFramework/` is `jpt.TestFramework`, and it is **one class**:
`TestCase`. Every level of a run is a plain list of calls, so opening
`Source/ApplicationUnitTests.cpp` and reading downwards reaches every test there is:

```
ApplicationUnitTests::Init()                        lists the 4 categories
  └── RunUnitTests_<Category>()                     names and runs that folder's cases
        └── TestCase::Run("Cat.Name", &RunUnitTests_<Name>)
```

A case is **two edits**: the function, plus one `import` and one `Run` line in the aggregator
beside it. That second edit is the point — it is what makes the list, rather than the linker,
decide what runs, and it is what lets a case be reordered or dropped without touching the case
itself.

```cpp
export module UnitTests_TwoSum;         // no jpt. prefix: project-local, nothing engine-side imports it
import jpt.TestFramework;

// No namespace, and deliberately so: a test file spells jpt:: where it means the engine, which
// keeps `TwoSum` here from reading as though it were `jpt::TwoSum`.
std::vector<int32> TwoSum(...) { ... }

// A case is a function taking the TestCase it reports to, and nothing else. There is no
// per-case wrapper: the display name lives at the aggregator's call site, so one file's cases
// cost one line each there and nothing here.
export void RunUnitTests_TwoSum(jpt::TestCase& test)
{
    test.Expect(TwoSum({ 2, 7 }, 9) == ..., "first two");
}
```

A file with several cases exports several such functions, and the aggregator lists them all —
`UnitTests_Matrix44` exports `RunUnitTests_Matrix44Transform` and `RunUnitTests_Matrix44Rotation`.
The function name is the registered name with the dot removed, so the two never drift apart. Since
these are exported into the global namespace, that name must be unique across the whole project.

`TestCase::Run` constructs a case, runs it, prints its `[ PASS ]`/`[ FAIL ]` line and folds its
checks into the run's totals; the constructor is private, so a case cannot exist outside a run.
`TestCase::Summarize()` prints the two totals lines and returns the verdict, and
`ApplicationUnitTests::Init()` calls it once. Note the scalar aliases (`int32`, `usize`,
`float32`) are **global**, not `jpt::` — only `Status` is in the namespace.

**There is no sort.** The old design registered each suite from a namespace-scope constructor and
sorted by name, because static-init order across TUs is unspecified. Ordering is now the order the
aggregators list, which is why `UnitTests_Core` runs the vectors before the matrix that composes
them and the quaternion after, and why `Coding.Scratch` is simply last instead of being named
`Scratch.Scratch` to sort there.

Aggregators return `void`. The verdict is not threaded back because `TestCase` already tallies
every check, so `Summarize()` is the single authority and each aggregator stays a pure list with no
`succeeded &=` noise. The tally is four file-scope `g_` counters in `TestFramework.cpp` — `g_`, not
`s_`, because a namespace-scope variable in a module unit has *module* linkage, so `static` is not
what it is. A tally, not a registry: nothing there decides what runs, only records what has run. A
failing case therefore never stops another; **the whole run always executes.**

**`TestCase::Expect` is `Debug::Assert` without the trap** — same `consteval Debug::Context`
parameter, so it needs no macro, reports the *call site's* file and line, and checks the format
string at compile time. It records the failure and returns the condition, so a run reports
everything broken rather than the first thing, and `Summarize()` returns `Status::Failed` — which
`Main.cpp` maps to `SDL_APP_FAILURE`, so **a failing check exits 1**. Being a function and not a
macro, a braced initializer with commas needs no defensive parentheses. Measured on 2026-09-02:
33 suites, 280 checks, exit 0; invert one check and it is 32/33, 279/280, exit 1.

Placement: a test for `Source/<Path>/Foo.cppm` lives at
`Projects/UnitTests/Source/<Path>/UnitTests_Foo.cppm` (`Coding/` is the one folder with no engine
counterpart), and each top-level folder owns one aggregator, `UnitTests_<Folder>`.

**A suite reaching `GetApp()` splits into an interface and an implementation unit** rather than
being a plain `.cpp`. `GetApp.h` carries `import jpt.ApplicationBase`, which in a `.cppm`
interface is a module-graph cycle — but an implementation unit's global module fragment takes it
fine, exactly as `Source/Graphics/RendererBase.cpp` does. `Input/UnitTests_SdlEvent.{cppm,cpp}` is
the only one: the `.cppm` declares `RunUnitTests_SdlEvent()` and imports nothing, the `.cpp`
includes `GetApp.h` and holds the body. It is also why the suites run from `Init()` rather than
`PreInit()`: it drives a live `Window`, and `SetStatus` is ignored until
`ApplicationBase::Init()` has set `Running`.

## Conventions

- C++23, no compiler extensions. Compiler pinned to `clang++` everywhere — assume clang, not MSVC.
- On Apple, the compiler must be **Homebrew LLVM** (`/opt/homebrew/opt/llvm/bin/clang++`) for both
  `CXX` and `OBJCXX`. Apple clang cannot build C++ modules at all: no `clang-scan-deps`, no
  `libc++.modules.json`. Homebrew clang links the system `libc++`, so nothing is embedded in an iOS bundle.
- On Windows, **llvm-mingw** (`clang++` on `PATH`), not clang-cl and not MSVC. What `import std;`
  needs is already there and needs no preset entry: it ships libc++, and
  `clang++ -print-file-name=libc++.modules.json` resolves to the per-target
  `x86_64-w64-mingw32/lib/` copy, which is what the `if(APPLE)` `CMAKE_CXX_STDLIB_MODULES_JSON`
  override exists to supply where that lookup fails. CMake 4.2 no longer gates `import std` behind
  `CMAKE_EXPERIMENTAL_CXX_IMPORT_STD`, so that line at the top of `CMakeLists.txt` is inert there
  and still load-bearing on an older CMake.
- On Android, the **NDK pinned in `Scripts/utils.py`**, driven through its own toolchain file by
  the android presets. The NDK ships no std module at all, so `Vendor/LibCxxModules` stands in —
  generated from the NDK's exact LLVM commit, and the two are bumped together or not at all; the
  README there records every measured trap (the no-`--target` probe that found llvm-mingw's
  std.cppm on `PATH`, Bionic's static-inline libc wrappers).
- **The three toolchains are versions apart** — clang 20 on Windows, 21 on the NDK, 22 on macOS.
  A module or `import std` failure on one and not the others is that gap before it is the code.
- Automation is cross-platform Python routed through `setup → build → run`. No `.sh`/`.bat`.
- Pointers take a `p` prefix (`m_pDevice`, `pDrawable`).
- Comment only what the code cannot say. Prefer a short trailing comment over a block that splits a
  run of related calls.

### Class layout

Sections in this order, each opened by its own access specifier (repeat `public:`/`private:`
freely; omit a section that would be empty):

1. `public:` constants — `static constexpr k...`, `enum class`, type aliases, and the consteval
   factories (`Zero()`, `Identity()`, `Black()`) that stand in for constants of the class's own type.
2. `private:` data members (public data in a plain struct sits in this same slot).
3. `public:` lifecycle — constructor, destructor, copy/move constructor and assignment, then
   `PreInit`, `Init`, `Update`, `PostUpdate`, `Terminate` in call order.
4. `public:` API — the `DoWork()` verbs, then event callbacks (`OnResize`, `OnKeyEvent`,
   `OnTouchEvent`, ...), then getters/setters.
5. `private:` functions — lifecycle helpers first in the same call order, then the rest.

**No function bodies inside a class.** Declare in the class, define elsewhere: in the `.cpp` for a
non-template, or at namespace scope below the class in the same `.cppm`/header when the definition
must stay visible to importers (templates, `constexpr`/`consteval`). `= default` and `= delete`
stay on the declaration, as does everything that C++ forbids repeating on a definition
(`[[nodiscard]]`, `virtual`, `static`, `explicit`, default arguments). `LinearColor.cppm` shows the
below-the-class split, `FrameTimer` the `.cppm`/`.cpp` split.

**Exception — a trivial accessor keeps its body on the declaration.** Splitting a line that *is*
the member costs a reader two files to learn nothing, so write it inline:

```cpp
[[nodiscard]] Renderer& GetRenderer() noexcept { return m_renderer; }
void SetFovY(float32 radians) noexcept { m_fovY = radians; }
[[nodiscard]] static consteval Vector3 Up() noexcept { return Vector3(0, 1, 0); }
```

Trivial means the whole body is one statement that returns a data member, forwards to that
member's own accessor (`Camera::Right` → `m_rotation.Right()`), assigns a parameter to one, or
names a constant of the class's own type. A cast on the way out is still trivial
(`FrameTimer::GetDeltaSeconds`).

Anything that **computes** is not, however short, and moves out under the rule above: arithmetic or
a branch (`Window::GetAspectRatio`, `Texture::RowPitch`, `Camera::SetDistance` clamping), a
call that builds a new value (`Path::GetFileName`, `Transform::ToMatrix`), reading two members at
once (`Input::IsKeyDown`), or a second statement (`Window::SetCursorCaptured`).

**Definitions in the `.cpp` follow the class's declaration order.**

### Prefer a class over free functions

**A group of free functions that share state, a lifecycle, or a subject is a class.** One public
entry point, everything else private — `EditorUI::Draw()` and `GpuCapture` are the shape. A reader
then learns the type once instead of tracing which file-scope variable each function mutates, and
the compiler stops anything outside from reaching the parts.

The tell is *shared state*: a `g_` variable that two functions read, or a parameter threaded by
reference through a chain of calls because it is really a cursor. Both were free functions here
before they were classes, and both were harder to follow for it.

Free is still right for these, so do not convert them:

- **Pure functions of their arguments** that touch no `this` — `ToFloat4`, `MipLevelCount`,
  `AreValuesClose`, `ToString(KeyCode)`. As private members they would gain access to state they
  must not use.
- **Operators**, which C++ requires to be free to allow the left-hand conversion.
- **The platform seam.** `ImGuiLayer.h`'s functions, `Main.cpp`'s four `SDL_App*` callbacks and
  `jpt::GetApp()` are free because module attachment forbids anything else — see the Modules section.

Prefer pulling over parameters when the method is already inside the engine: `EditorUI`'s sections
take no arguments and reach what they edit through `GetApp()`, which is what keeps
`EditorUI.cppm` free of `Camera`, `Material` and `PointLight` imports.

**`namespace local` is where a file's own helpers live** — whatever is not a member of the type the
file implements. It replaces the anonymous namespace, so every parked helper is one grep away. The
rule it serves: opening a `.cpp` shows the bodies of the type you came for, never helper machinery
first. In order of preference:

1. **A massive helper set becomes its own module** (`Metal4Helpers.cppm/.cpp`). Exported `jpt` free
   functions, not `local` — a plain `.cpp` importer cannot reach an unexported declaration.
2. **Declarations at the bottom of the `.cppm`, below the `export` block; bodies at the bottom of
   the `.cpp`** (`ObjLoader`, `TextureLoader`). Unexported, which clang enforces — *"declaration of
   'ParseCorner' must be imported from module"* — and the only plain way two implementation units
   of one module share a helper.
3. **No interface unit, just a `.h`?** A shared header is no place for private helpers, so declare
   the `local` signatures at the top of the `.cpp` and define the bodies at its bottom
   (`Window.cpp`).

Two mechanics: a named namespace has no implicit using-directive, so calls stay qualified
`local::`; and it grants no internal linkage, so two plain `.cpp`s sharing a `local` name is a
duplicate-symbol link error (module units are immune — module linkage).

The `.cppm` split costs rebuild fan-out: a `.cpp` edit rebuilds one TU, a `.cppm` edit rebuilds the
module and every importer — measured at 1 vs 3 for `jpt.ObjLoader`, 26 for `jpt.Vector3`. If a
widely imported module's `local` churns, keep it in the `.cpp`.

### Python (`Scripts/`)

- Docstrings per [PEP 257](https://peps.python.org/pep-0257/) — a `"""..."""` as the first statement,
  never a banner comment above `def` (that leaves `__doc__` as `None`).
- Annotate return types on every function that returns a value.
- Shared constants and path helpers live in `utils.py`; `artifact_path()` is the single place that
  knows a macOS binary sits inside `.app/Contents/MacOS/` while an iOS `.app` installs whole.
- `Scripts/` holds only `setup`, `build`, `run`, `clean` and `utils.py`; anything else goes in a
  subfolder and needs the `sys.path` line described under Build.
- Scripts take arguments only and never prompt. On bad input, print `utils.USAGE` and exit non-zero.
