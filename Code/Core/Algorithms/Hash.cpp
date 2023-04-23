#include "JupiterPCH.h"
#include "Hash.h"

constexpr uint32 jpt::StringHash32(const char* const str, const uint32 value) noexcept
{
    return (str[0] == '\0') ? value : StringHash32(&str[1], (value ^ uint32(str[0])) * kPrime32);
}

constexpr uint64 jpt::StringHash64(const char* const str, const uint64 value) noexcept
{
    return (str[0] == '\0') ? value : StringHash64(&str[1], (value ^ uint64(str[0])) * kPrime64);
}

