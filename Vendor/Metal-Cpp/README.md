# metal-cpp

Apple's header-only C++ bindings for Metal, Foundation, QuartzCore and MetalFX.

- Source: https://github.com/apple/metal-cpp
- Tag: `release/metal-cpp_macOS26.4_iOS26.4` (matches the installed 26.5 SDK; the `macOS27`
  tag targets an SDK that is not installed)

`Include/` is gitignored. To restore it:

```
git clone --depth 1 --branch release/metal-cpp_macOS26.4_iOS26.4 \
    https://github.com/apple/metal-cpp.git /tmp/metal-cpp
mkdir -p Vendor/Metal-Cpp/Include
cp -R /tmp/metal-cpp/{Foundation,Metal,QuartzCore,MetalFX} Vendor/Metal-Cpp/Include/
```

## Notes

`Source/Graphics/Metal/MetalRenderer.cpp` is the single translation unit that defines
`NS_PRIVATE_IMPLEMENTATION` / `CA_PRIVATE_IMPLEMENTATION` / `MTL_PRIVATE_IMPLEMENTATION`.
If a second TU ever needs these headers, move those defines into a dedicated impl file.

There is no ARC. Owning references use `NS::SharedPtr` with `NS::TransferPtr`; everything
else is autoreleased and needs an `NS::AutoreleasePool` around the frame.

`CA::MetalLayer` covers `device`, `pixelFormat`, `framebufferOnly`, `drawableSize`,
`nextDrawable`, `maximumDrawableCount`, `displaySyncEnabled`, `colorspace`. It does **not**
cover `wantsExtendedDynamicRangeContent` or `presentsWithTransaction` — set those from
ObjC where the layer is created. There are no AppKit, UIKit, MetalKit or CADisplayLink
bindings, which is why the window layer stays `.mm`.
