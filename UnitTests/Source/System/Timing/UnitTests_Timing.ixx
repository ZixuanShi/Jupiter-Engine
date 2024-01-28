// Copyright Jupiter Technologies, Inc. All Rights Reserved.

module;

#include "Core/Minimals/Headers.h"

export module UnitTests_Timing;

import jpt.CoreModules;
import jpt.Timing.Utils;

bool UnitTest_Timing()
{
    JPT_LOG(jpt::ToString(jpt::Timing::Now()));

    return true;
}

export bool RunUnitTests_Timing()
{
    JPT_RETURN_FALSE_IF_ERROR(!UnitTest_Timing(), "UnitTest_Timing Failed");

    return true;
}
