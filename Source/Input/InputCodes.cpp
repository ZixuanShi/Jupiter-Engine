// Copyright Jupiter Technologies, Inc. All Rights Reserved.

module jpt.InputCodes;

import jpt.TypeDefs;
import std;

namespace jpt
{
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
