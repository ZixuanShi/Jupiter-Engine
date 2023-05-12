#pragma once

namespace jpt
{
    // Returns: The length of inString. aka How many characters in it
    JPT_API inline size_t strlen(const char* inString);

    // Converter from int to string
    // - value: The integer value to convert
    // - base: System of numeration. Octal, decimal, hexadecimal, etc. Default to decimal
    JPT_API inline char* itoa(int32 value, int32 base = 10);

    // Converts an input object to string if it has this functionality
    // Primitive types should be specialized below
    template<class Type>
    inline jpt::string to_string(const Type& object)
    {
        return object.to_string();
    }
    template<>
    inline jpt::string to_string<int32>(const int32& num)
    {
        return jpt::string(jpt::itoa(num));
    }

    // FNV1a c++11 constexpr compile time hash functions, 32 and 64 bit
    // str should be a null terminated string literal, value should be left out 
    // e.g hash_32_fnv1a_const("example")
    // code license: public domain or equivalent
    // post: https://notes.underscorediscovery.com/constexpr-fnv1a/
    // https://gist.github.com/underscorediscovery/81308642d0325fd386237cfa3b44785c#file-hash_fnv1a_constexpr-h    
    JPT_API inline constexpr uint32 StringHash32(const char* const str, const uint32 value = 0x811c9dc5) noexcept;
    JPT_API inline constexpr uint64 StringHash64(const char* const str, const uint64 value = 0xcbf29ce484222325) noexcept;
}