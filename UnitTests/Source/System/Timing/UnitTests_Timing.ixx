// Copyright Jupiter Technologies, Inc. All Rights Reserved.

module;

#include "Core/Minimals/Headers.h"

export module UnitTests_Timing;

import jpt.CoreModules;

bool UnitTest_Timing()
{

    return true;
}

export bool RunUnitTests_Timing()
{
    JPT_RETURN_FALSE_IF_ERROR(!UnitTest_Timing(), "UnitTest_Timing Failed");

    return true;
}
