// Copyright Jupiter Technologies, Inc. All Rights Reserved.

#include "JupiterPCH.h"
#include "String.h"

namespace jpt
{
    size_t strlen(const char* inString)
    {
        if (!inString)
        {
            return 0;
        }

        size_t size = 0;
        while (inString[size] != '\0')
        {
            ++size;
        }

        return size;
    }

    size_t wcslen(const wchar_t* inString)
    {
        if (!inString)
        {
            return 0;
        }

        size_t size = 0;
        while (inString[size] != L'\0')
        {
            ++size;
        }

        return size;
    }

    int32 strncmp(const char* string1, const char* string2, size_t size)
    {
        for (size_t i = 0; i < size; ++i)
        {
            if (string1[i] != string2[i])
            {
                return kInvalidValue<int32>;
            }
        }
        return 0;
    }

    int32 wcsncmp(const wchar_t* string1, const wchar_t* string2, size_t size)
    {
        for (size_t i = 0; i < size; ++i)
        {
            if (string1[i] != string2[i])
            {
                return kInvalidValue<int32>;
            }
        }
        return 0;
    }
}