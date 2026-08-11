// Copyright Jupiter Technologies, Inc. All Rights Reserved.

module jpt.InputCodes;

import jpt.TypeDefs;
import jpt.Vector2;
import std;

namespace jpt
{
    Vec2 ToTouchPixels(float32 normalizedX, float32 normalizedY, TouchDevice device,
                       uint32 width, uint32 height) noexcept
    {
        const float32 heightF = static_cast<float32>(height);
        const float32 y = normalizedY * heightF;

        // IndirectAbsolute alone is normalized over the pad, and a non-square pad makes that
        // anisotropic. SDL exposes no deviceSize, so a measured constant stands in -- Apple
        // trackpads are ~1.6:1, and without it a physical 45 degree twist reads about 32.
        if (device == TouchDevice::IndirectAbsolute)
        {
            constexpr float32 kPadAspect = 1.6f;
            return Vec2(normalizedX * heightF * kPadAspect, y);
        }

        return Vec2(normalizedX * static_cast<float32>(width), y);
    }

    const char* ToString(KeyCode key) noexcept
    {
        const usize index = static_cast<usize>(key);
        return (index < std::size(local::kKeyNames)) ? local::kKeyNames[index] : "Unknown";
    }

    const char* ToString(MouseButton button) noexcept
    {
        const usize index = static_cast<usize>(button);
        return (index < std::size(local::kMouseButtonNames)) ? local::kMouseButtonNames[index] : "Unknown";
    }
}
