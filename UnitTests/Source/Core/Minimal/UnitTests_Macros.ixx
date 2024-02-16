// Copyright Jupiter Technologies, Inc. All Rights Reserved.

module;

#include "Core/Minimal/Headers.h"

#include <stdarg.h>
#include <stdio.h>

export module UnitTests_Macros;

import jpt.StringUtils;

bool UnitTest_Macro_String()
{
    auto helper = []<size_t kSize = 32>(const char* expectedResult, const char* format, ...) -> bool
        {
            char buffer[kSize];
            JPT_FORMAT_STRING(buffer, format, ...);
        
            return jpt::AreStringsSame(buffer, expectedResult);
        };

    JPT_ENSURE(helper("Hello World", "Hello World"), "");
    JPT_ENSURE(helper("42", "%d", 42), "");

    return true;
}

bool UnitTest_Macro_VA_ARGS()
{
    JPT_HAS_ARGS();

    return true;
}

export bool RunUnitTests_Macros()
{
    JPT_ENSURE(UnitTest_Macro_String(), "UnitTest_Macro_String Failed");
    JPT_ENSURE(UnitTest_Macro_VA_ARGS(), "UnitTest_Macro_VA_ARGS Failed");

    return true;
}
