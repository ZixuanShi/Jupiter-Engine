// Copyright Jupiter Technologies, Inc. All Rights Reserved.

module;

#include "Core/Minimal/CoreHeaders.h"

export module UnitTests_CSV;

import jpt.CSV;
import jpt.TypeDefs;
import jpt.Utilities;

bool UnitTests_CSV()
{
    return true;
}

export bool RunUnitTests_CSV()
{
    JPT_ENSURE(UnitTests_CSV());

    return true;
}
