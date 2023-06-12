// Copyright Jupiter Technologies, Inc. All Rights Reserved.

#pragma once

namespace jpt
{
    /* @return the input const char* string's size */
    JPT_API inline size_t strlen(const char* inString);
    JPT_API inline size_t wcslen(const wchar_t* inString);

    /* Convert from int32 to a char pointer holding the integer's value as string literal 
        @param value:        The value to convert to char*
        @param base:         The base of the value. Default to decimal as 10. Could be binary, oct, hex. 
        @return A char pointer pointing to the memory where we store the converted number's string literal */
    JPT_API inline char* itoa(int32 value, int32 base = 10);

    /* Compares two strings from the beginnings to the given size
        @return 0 if two strings are identical. InvalidValue if not */
    JPT_API inline int32 strncmp(const char* string1, const char* string2, size_t size);
    JPT_API inline int32 wcsncmp(const wchar_t* string1, const wchar_t* string2, size_t size);

    // FNV1a c++11 constexpr compile time hash functions, 32 and 64 bit
    // str should be a null terminated string literal, value should be left out 
    // e.g hash_32_fnv1a_const("example")
    // code license: public domain or equivalent
    // post: https://notes.underscorediscovery.com/constexpr-fnv1a/
    // https://gist.github.com/underscorediscovery/81308642d0325fd386237cfa3b44785c#file-hash_fnv1a_constexpr-h    
    JPT_API inline constexpr uint32 StringHash32(const char* const str, const uint32 value = 0x811c9dc5) noexcept;
    JPT_API inline constexpr uint64 StringHash64(const char* const str, const uint64 value = 0xcbf29ce484222325) noexcept;

    template<>
    struct hash<const char*>
    {
        size_t operator()(const char* key)
        {
            return jpt::StringHash64(key);
        }
    };

    /* Calculate the inString's size
        @param inString:	Templated const chartype string to check for size. */
    template<typename CharType>
    inline size_t GetStrLength(const CharType* inString)
    {
        if constexpr (jpt::IsSameType<CharType, char>::Value)
        {
            return jpt::strlen(inString);
        }
        else if (jpt::IsSameType<CharType, wchar_t>::Value)
        {
            return jpt::wcslen(inString);
        }
    }
}