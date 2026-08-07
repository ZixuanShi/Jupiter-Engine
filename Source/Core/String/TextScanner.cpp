// Copyright Jupiter Technologies, Inc. All Rights Reserved.

module jpt.TextScanner;

import jpt.StringUtils;
import jpt.TypeDefs;
import std;

namespace jpt
{
    TextScanner::TextScanner(std::string_view text) noexcept
        : m_text(text)
    {
    }

    void TextScanner::SkipBlanks() noexcept
    {
        const usize start = m_text.find_first_not_of(" \t");
        m_text.remove_prefix(start == std::string_view::npos ? m_text.size() : start);
    }

    bool TextScanner::Consume(char c) noexcept
    {
        const bool present = m_text.starts_with(c);
        if (present)
        {
            m_text.remove_prefix(1);
        }
        return present;
    }

    bool TextScanner::Consume(std::string_view prefix) noexcept
    {
        const bool present = m_text.starts_with(prefix);
        if (present)
        {
            m_text.remove_prefix(prefix.size());
        }
        return present;
    }

    bool TextScanner::Parse(int32& value) noexcept
    {
        SkipBlanks();

        const std::from_chars_result result = std::from_chars(m_text.data(), m_text.data() + m_text.size(), value);
        if (result.ec != std::errc{})
        {
            return false;
        }

        m_text.remove_prefix(static_cast<usize>(result.ptr - m_text.data()));
        return true;
    }

    bool TextScanner::Parse(float32& value) noexcept
    {
        SkipBlanks();

        TextScanner cursor = *this;
        const bool negative = cursor.ParseSign();

        // Accumulated as one integer and scaled once, rather than adding each fraction
        // digit at its own magnitude -- half the rounding steps for the same digits.
        float64 mantissa = 0.0;
        usize digits = cursor.ParseDigits(mantissa);

        if (cursor.Consume('.'))
        {
            const usize beforeFraction = cursor.m_text.size();
            digits += cursor.ParseDigits(mantissa);
            mantissa /= std::pow(10.0, static_cast<float64>(beforeFraction - cursor.m_text.size()));
        }

        if (digits == 0)
        {
            return false;
        }

        // Advances past the exponent only if digits follow the 'e'; "1.5elephant" keeps "1.5".
        TextScanner exponentCursor = cursor;
        if (exponentCursor.Consume('e') || exponentCursor.Consume('E'))
        {
            const bool exponentNegative = exponentCursor.ParseSign();

            float64 exponent = 0.0;
            if (exponentCursor.ParseDigits(exponent) > 0)
            {
                mantissa *= std::pow(10.0, exponentNegative ? -exponent : exponent);
                cursor = exponentCursor;
            }
        }

        value = static_cast<float32>(negative ? -mantissa : mantissa);
        m_text = cursor.m_text;
        return true;
    }

    bool TextScanner::ParseSign() noexcept
    {
        const bool negative = Consume('-');
        if (!negative)
        {
            (void)Consume('+');
        }
        return negative;
    }

    usize TextScanner::ParseDigits(float64& value) noexcept
    {
        usize count = 0;
        for (; !m_text.empty() && IsDigit(m_text.front()); ++count)
        {
            value = value * 10.0 + static_cast<float64>(m_text.front() - '0');
            m_text.remove_prefix(1);
        }
        return count;
    }
}
