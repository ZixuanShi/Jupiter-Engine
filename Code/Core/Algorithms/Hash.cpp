#include "JupiterPCH.h"
#include "Hash.h"

namespace jpt
{
    constexpr uint32 StringHash32(const char* const str, const uint32 value) noexcept
    {
        return (str[0] == '\0') ? value : StringHash32(&str[1], (value ^ uint32(str[0])) * kPrime32);
    }

    constexpr uint64 StringHash64(const char* const str, const uint64 value) noexcept
    {
        return (str[0] == '\0') ? value : StringHash64(&str[1], (value ^ uint64(str[0])) * kPrime64);
    }
}

