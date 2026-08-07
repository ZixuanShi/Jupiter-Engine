// Copyright Jupiter Technologies, Inc. All Rights Reserved.

#pragma once

#include <cstdint>

namespace jpt
{
    class WindowBase
    {
    protected:
        std::uint32_t m_width  = 0;
        std::uint32_t m_height = 0;

    public:
        void OnResize(std::uint32_t pixelWidth, std::uint32_t pixelHeight) noexcept;

        [[nodiscard]] std::uint32_t GetWidth()  const noexcept { return m_width; }
        [[nodiscard]] std::uint32_t GetHeight() const noexcept { return m_height; }
        [[nodiscard]] float GetAspectRatio() const noexcept;
    };
}
