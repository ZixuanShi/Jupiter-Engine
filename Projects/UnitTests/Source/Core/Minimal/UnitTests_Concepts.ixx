// Copyright Jupiter Technologies, Inc. All Rights Reserved.

module;

#include "Core/Minimal/CoreHeaders.h"

export module UnitTests_Concepts;

import jpt.Concepts;
import jpt.TypeDefs;
import jpt.TypeTraits;
import jpt.Utilities;
import jpt.Vector2;
import jpt.Vector3;

template<typename T>
bool IsTrivial()
{
    return jpt::IsTrivial<T>;
}

bool UnitTests_Trivial()
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
    JPT_ENSURE(UnitTests_Trivial());

    return true;
}
