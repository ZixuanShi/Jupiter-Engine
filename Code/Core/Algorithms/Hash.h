#pragma once

namespace jpt
{
    // Constexpr string hashing from:
    // https://gist.github.com/underscorediscovery/81308642d0325fd386237cfa3b44785c#file-hash_fnv1a_constexpr-h
    // With some changes to naming according to GAP coding standards
    
    // FNV1a c++11 constexpr compile time hash functions, 32 and 64 bit
    // str should be a null terminated string literal, value should be left out 
    // e.g hash_32_fnv1a_const("example")
    // code license: public domain or equivalent
    // post: https://notes.underscorediscovery.com/constexpr-fnv1a/
    
    static constexpr uint32 kVal32   = 0x811c9dc5;
    static constexpr uint32 kPrime32 = 0x1000193;
    static constexpr uint64 kVal64   = 0xcbf29ce484222325;
    static constexpr uint64 kPrime64 = 0x100000001b3;
    
    inline constexpr uint32 StringHash32(const char* const str, const uint32 value = kVal32) noexcept;    
    inline constexpr uint64 StringHash64(const char* const str, const uint64 value = kVal64) noexcept;
}