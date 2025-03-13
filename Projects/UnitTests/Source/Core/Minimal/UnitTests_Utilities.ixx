// Copyright Jupiter Technologies, Inc. All Rights Reserved.

module;

#include "Core/Minimal/CoreHeaders.h"

export module UnitTests_Utilities;

import jpt.Utilities;
import jpt.String;

bool UnitTests_Move()
{
    // String
    jpt::String str1 = "Jupiter";
    jpt::String str2 = "Engine";
    str1 = jpt::Move(str2);
    JPT_ENSURE(str1 == "Engine");
    JPT_ENSURE(str2.IsEmpty());
    JPT_ENSURE(!str2.ConstBuffer());

    return true;
}

export bool RunUnitTests_Utilities()
{
    JPT_ENSURE(UnitTests_Move());

    return true;
}