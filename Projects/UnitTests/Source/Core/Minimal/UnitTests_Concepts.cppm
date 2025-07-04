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

template<jpt::Trivial T>
bool IsTrivial()
{
    return true;
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

JPT_ENUM_UINT8(EAxis, X, Y, Z);

template<jpt::JptEnumerated T>
bool IsEnumerated()
{
    return true;
}

template<typename T> requires (!jpt::JptEnumerated<T>)
bool IsEnumerated()
{
    return false;
}

bool UnitTests_Enumerated()
{
    JPT_ENSURE(IsEnumerated<EAxis>());

    JPT_ENSURE(!IsEnumerated<uint32>());
    JPT_ENSURE(!IsEnumerated<jpt::String>());

    return true;
}

export bool RunUnitTests_Concepts()
{
    JPT_ENSURE(UnitTests_Trivial());
    JPT_ENSURE(UnitTests_Enumerated());

    return true;
}
