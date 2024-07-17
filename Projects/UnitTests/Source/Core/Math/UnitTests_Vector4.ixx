// Copyright Jupiter Technologies, Inc. All Rights Reserved.

module;

#include "Core/Minimal/CoreHeaders.h"

export module UnitTests_Vector4;

import jpt.Vector4;
import jpt.String;
import jpt.TypeDefs;
import jpt.Utilities;

bool UnitTests_Vector4()
{
    Vec4f v1(1.0f, 2.0f, 3.0f, 4.0f);
    //JPT_LOG(v1);
    return true;
}

export bool RunUnitTests_Vector4()
{
    JPT_ENSURE(UnitTests_Vector4());

    return true;
}
