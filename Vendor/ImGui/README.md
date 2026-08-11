# Dear ImGui

Immediate-mode GUI, used for the editor panels under `Source/Editor/`.

- Source: https://github.com/ocornut/imgui
- Branch: `docking` — dockable panels are the base a level editor needs, and the docking API is
  additive, so code written against it will not compile on `master`.
- Pinned commit: `b48d1afbe8ee8b238e2961dc363a949dd7304e23` (2026-07-31)

`Source/` is gitignored. To restore it:

```
git clone --depth 1 --branch docking https://github.com/ocornut/imgui.git /tmp/imgui
mkdir -p Vendor/ImGui/Source/backends
cp /tmp/imgui/{imconfig.h,imgui.cpp,imgui.h,imgui_demo.cpp,imgui_draw.cpp,imgui_internal.h,imgui_tables.cpp,imgui_widgets.cpp,imstb_rectpack.h,imstb_textedit.h,imstb_truetype.h} Vendor/ImGui/Source/
cp /tmp/imgui/backends/imgui_impl_metal4.{h,mm} Vendor/ImGui/Source/backends/
cp /tmp/imgui/backends/imgui_impl_sdl3.{h,cpp} Vendor/ImGui/Source/backends/
```

## Notes

Built as its own static library, `JupiterImGui`, for three reasons: it needs `-w` (it does not
survive this project's `-Wall -Wextra -Wpedantic -Werror`), it needs `CXX_SCAN_FOR_MODULES OFF`
so `clang-scan-deps` never walks vendor code, and it is skipped entirely in Release.

**Two backends, both cross-platform.** `imgui_impl_metal4.mm` draws; `imgui_impl_sdl3.cpp` feeds
it input, display size and delta time. Both build for macOS and iOS from one source list, and
`imgui_impl_sdl3.cpp` is what replaced the macOS-only `imgui_impl_osx.mm` along with the
hand-written iOS touch-to-mouse path.

The render backend is Objective-C Metal while the renderer is metal-cpp. They are the same objects
— metal-cpp is a typed view over `objc_msgSend` — so `Source/Graphics/ImGui/ImGuiLayer.mm` bridges
with `(__bridge id<MTLDevice>)(void*)pDevice`, the same trick `Window.cpp` uses in the other
direction on the `CA::MetalLayer` that `SDL_Metal_GetLayer` returns.

**`imgui_impl_metal4.mm`, not `imgui_impl_metal.mm`** -- the engine renders through Metal 4, and the
classic backend encodes into an `MTLRenderCommandEncoder` the renderer never creates. It needs two
things the classic one does not: the command queue at init, and a frame-in-flight index per frame,
because it sizes its own vertex/index/constant buffers off them. `Metal4Renderer` passes it the
same `kFramesInFlight` and the same ring slot it used for the command allocator; they must agree.
