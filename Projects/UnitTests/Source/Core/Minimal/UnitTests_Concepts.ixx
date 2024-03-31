// Copyright Jupiter Technologies, Inc. All Rights Reserved.

module;

#include "Core/Minimal/Headers.h"

export module UnitTests_Concepts;

import jpt.CoreModules;
import jpt.Concepts;
import jpt.TypeDefs;
import jpt.TypeTraits;
import jpt.Utilities;

template<jpt::Trivial T>
bool IsTrivial()
{
    return true;
}

bool UnitTest_Trivial()
{
    JPT_ENSURE(IsTrivial<int32>());
    JPT_ENSURE(IsTrivial<int64>());
    JPT_ENSURE(IsTrivial<uint32>());
    JPT_ENSURE(IsTrivial<uint64>());
    JPT_ENSURE(IsTrivial<float32>());
    JPT_ENSURE(IsTrivial<float64>());
    JPT_ENSURE(IsTrivial<Vec2f>());
    JPT_ENSURE(IsTrivial<Vec3f>());

    return true;
}

export bool RunUnitTests_Concepts()
{
    JPT_ENSURE(UnitTest_Trivial());

    return true;
}
