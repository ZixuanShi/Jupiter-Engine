// Copyright Jupiter Technologies, Inc. All Rights Reserved.

module;

#include "Core/Minimal/CoreHeaders.h"
#include "Core/Strings/StringMacros.h"

#include <stdarg.h>
#include <stdio.h>
#include <initializer_list>

export module UnitTests_Macros;

import jpt.String.Helpers;
import jpt.Utilities;

#pragma region String Formatting
template<size_t kCount = 32>
bool TestFormatHelper(const char* expectedResult, const char* format, ...)
{
    char buffer[kCount];
    JPT_FORMAT_STRING(buffer, format, ...);

    return jpt::AreStringsSame(buffer, expectedResult);
}

bool UnitTests_Macro_String()
{
    JPT_ENSURE(TestFormatHelper("Hello World", "Hello World"));
    JPT_ENSURE(TestFormatHelper("42", "%d", 42));
    JPT_ENSURE(TestFormatHelper<128>("Thursday, December 28, 2023, 10:16:56 PM - Jupiter Engine's Birthday", "%s, %s %d, %d, %d:%d:%d %s - %s", "Thursday", "December", 28, 2023, 10, 16, 56, "PM", "Jupiter Engine's Birthday"));

    return true;
}
#pragma endregion

#pragma region Has Args

// Parameter Pack. Can handle any type of arguments.
void ParameterPack()
{
}

template<typename T>
void ParameterPack(T t)
{
    JPT_LOG(t);
}

template<typename T, typename ...TArgs>
void ParameterPack(T t, TArgs&&... args)
{
    JPT_LOG(t);
    ParameterPack(args...);
}

// Parameter Pack Lambda. Can handle any type of arguments.
void ParameterPackLambda()
{
}

template<typename ...TArgs>
void ParameterPackLambda(TArgs&&... args)
{
    ([&]
        {
            JPT_LOG(args);
        } (), ...);
}

// Initializer List. Can only handle one type, and non-empty arguments.
template<typename T>
void InitializerList(const std::initializer_list<T>& list)
{
    for (const T& item : list)
    {
        JPT_LOG(item);
    }
}

bool UnitTests_Macro_VA_ARGS()
{
    JPT_ENSURE(JPT_HAS_ARGS() == false);
    JPT_ENSURE(JPT_HAS_ARGS(false));
    JPT_ENSURE(JPT_HAS_ARGS(1, 2, 3));
    JPT_ENSURE(JPT_HAS_ARGS(1, "2", 3, 'c'));

    return true;
}

#pragma endregion

#pragma region Args Count

bool UnitTests_Macro_ArgsCount()
{
    JPT_ENSURE(JPT_ARGS_COUNT() == 0);
    JPT_ENSURE(JPT_ARGS_COUNT(false) == 1);
    JPT_ENSURE(JPT_ARGS_COUNT(1, 2, 3) == 3);
    JPT_ENSURE(JPT_ARGS_COUNT(1, "2", 3, 'c') == 4);

    return true;
}

#pragma endregion

export bool RunUnitTests_Macros()
{
    JPT_ENSURE(UnitTests_Macro_String());
    JPT_ENSURE(UnitTests_Macro_VA_ARGS());
    JPT_ENSURE(UnitTests_Macro_ArgsCount());

    return true;
}
