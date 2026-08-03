// Copyright Jupiter Technologies, Inc. All Rights Reserved.

export module jpt.StringUtils;

export namespace jpt
{
    [[nodiscard]] constexpr bool IsDigit(char c) noexcept
    {
        return c >= '0' && c <= '9';
    }
}
