// Copyright Jupiter Technologies, Inc. All Rights Reserved.

export module jpt.TextScanner;

import jpt.TypeDefs;
import std;

export namespace jpt
{
    /** A cursor over borrowed text. Every Parse/Consume stops at the first character that is
        not part of what it reads, which is what lets one call read a field out of "12/3/4"
        and leave the separator behind. */
    class TextScanner
    {
    private:
        std::string_view m_text;

    public:
        explicit TextScanner(std::string_view text) noexcept;

    public:
        void SkipBlanks() noexcept;

        /** Consumes the prefix if present. */
        [[nodiscard]] bool Consume(char c) noexcept;
        [[nodiscard]] bool Consume(std::string_view prefix) noexcept;

        [[nodiscard]] bool Parse(int32& value) noexcept;

        /** Hand-rolled because libc++ puts the floating-point from_chars in the dylib behind a
            macOS 26 availability guard, which our deployment target does not clear. strtof is
            no substitute: it reads the decimal point from a locale, and taking no end pointer
            it would run off the end of one line and into the next. */
        [[nodiscard]] bool Parse(float32& value) noexcept;

    private:
        /** Consumes a leading '+' or '-' if there is one. */
        [[nodiscard]] bool ParseSign() noexcept;

        /** Consumes a run of digits into value, most significant first. Returns how many. */
        usize ParseDigits(float64& value) noexcept;
    };
}
