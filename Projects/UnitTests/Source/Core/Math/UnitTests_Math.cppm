// Copyright Jupiter Technologies, Inc. All Rights Reserved.

module;

#include "Core/Minimal/CoreHeaders.h"

export module UnitTests_Math;

import jpt.Math;
import jpt.StaticHashMap;
import jpt.Sort;
import jpt.TypeDefs;
import jpt.TypeTraits;
import jpt.Utilities;

bool UnitTests_Clamping()
{
    int32 n = jpt::Clamp(5, 10, 13);
    JPT_ENSURE(n == 10);

    n = jpt::Clamp(n, 5, 13);
    JPT_ENSURE(n == 10);

    n = jpt::Clamp(n, 1, 3);
    JPT_ENSURE(n == 3);

    JPT_ENSURE(jpt::Saturate(-1.0f) == 0.0f);
    JPT_ENSURE(jpt::Saturate(0.0f) == 0.0f);
    JPT_ENSURE(jpt::Saturate(0.5f) == 0.5f);
    JPT_ENSURE(jpt::Saturate(2.0f) == 1.0f);

    return true;
}

bool UnitTests_Interpolation()
{
    JPT_ENSURE(jpt::Lerp(0.0f, 10.0f, 0.0f) == 0.0f);
    JPT_ENSURE(jpt::Lerp(0.0f, 10.0f, 0.5f) == 5.0f);
    JPT_ENSURE(jpt::Lerp(0.0f, 10.0f, 0.75f) == 7.5f);
    JPT_ENSURE(jpt::Lerp(0.0f, 10.0f, 1.0f) == 10.0f);

    JPT_ENSURE(jpt::Lerp(-1.0f, 1.0f, 0.0f) == -1.0f);
    JPT_ENSURE(jpt::Lerp(-1.0f, 1.0f, 0.5f) == 0.0f);
    JPT_ENSURE(jpt::Lerp(-1.0f, 1.0f, 0.75f) == 0.5f);
    JPT_ENSURE(jpt::Lerp(-1.0f, 1.0f, 1.0f) == 1.0f);

    JPT_ENSURE(jpt::InvLerp(0.0f, 10.0f, 0.0f) == 0.0f);
    JPT_ENSURE(jpt::InvLerp(0.0f, 10.0f, 5.0f) == 0.5f);
    JPT_ENSURE(jpt::InvLerp(0.0f, 10.0f, 7.5f) == 0.75f);
    JPT_ENSURE(jpt::InvLerp(0.0f, 10.0f, 10.0f) == 1.0f);

    JPT_ENSURE(jpt::InvLerp(-1.0f, 1.0f, -1.0f) == 0.0f);
    JPT_ENSURE(jpt::InvLerp(-1.0f, 1.0f, 0.0f) == 0.5f);
    JPT_ENSURE(jpt::InvLerp(-1.0f, 1.0f, 0.5f) == 0.75f);
    JPT_ENSURE(jpt::InvLerp(-1.0f, 1.0f, 1.0f) == 1.0f);

    JPT_ENSURE(jpt::SmoothStep(0.0f, 1.0f, 0.0f) == 0.0f);
    JPT_ENSURE(jpt::AreValuesClose(jpt::SmoothStep(0.0f, 1.0f, 0.5f), 0.5f));
    JPT_ENSURE(jpt::SmoothStep(0.0f, 1.0f, 1.0f) == 1.0f);

    struct Answers
    {
        float32 lerp = 0.0f;
        float32 smooth = 0.0f;
        float32 smoother = 0.0f;
    };

    static const jpt::StaticHashMap<int32, Answers, 11> kResults = 
    {
        {  0,   {  0.0f, 0.000f, 0.000f } },
        {  1,   {  0.1f, 0.028f, 0.008f } },
        {  2,   {  0.2f, 0.104f, 0.058f } },
        {  3,   {  0.3f, 0.216f, 0.163f } },
        {  4,   {  0.4f, 0.352f, 0.317f } },
        {  5,   {  0.5f, 0.500f, 0.500f } },
        {  6,   {  0.6f, 0.648f, 0.683f } },
        {  7,   {  0.7f, 0.784f, 0.837f } },
        {  8,   {  0.8f, 0.896f, 0.942f } },
        {  9,   {  0.9f, 0.972f, 0.991f } },
        { 10,   {  1.0f, 1.000f, 1.000f } }
    };

    for (int32 i = 0; i <= 10; ++i) 
    {
        const float32 x = i / 10.0f;

        const float32 lerp     = jpt::Lerp(0.0f, 1.0f, x);
        const float32 smooth   = jpt::SmoothStep(0.0f, 1.0f, x);
        const float32 smoother = jpt::SmootherStep(0.0f, 1.0f, x);

        JPT_ENSURE(jpt::AreValuesClose(kResults[i].lerp, lerp));
        JPT_ENSURE(jpt::AreValuesClose(kResults[i].smooth, smooth, 0.001f));
        JPT_ENSURE(jpt::AreValuesClose(kResults[i].smoother, smoother, 0.001f));
    }

    return true;
}

bool UnitTests_Abs()
{
    JPT_ENSURE(jpt::Abs(-10) == 10);
    JPT_ENSURE(jpt::Abs(10) == 10);
    JPT_ENSURE(jpt::AreValuesClose(jpt::Abs(-10.55f), 10.55f));
    JPT_ENSURE(jpt::AreValuesClose(jpt::Abs(10.55f), 10.55f));
    return true;
}

bool UnitTests_AreValuesClose()
{
    JPT_ENSURE(jpt::AreValuesClose(10.556677f, 10.556677f));
    JPT_ENSURE(jpt::AreValuesClose(10.556677f, 10.956677f, 1.0f));

    JPT_ENSURE(jpt::AreValuesClose(42, 42));
    JPT_ENSURE(!jpt::AreValuesClose(42, 43));
    JPT_ENSURE(jpt::AreValuesClose(42, 43, 1));
    JPT_ENSURE(!jpt::AreValuesClose(42, 44, 1));
    JPT_ENSURE(jpt::AreValuesClose(42, 44, 2));

    return true;
}

class Foo
{
public:
    Foo() {}
    Foo(const Foo&) {}

    bool operator>(const Foo&) const { return true; }
    bool operator<(const Foo&) const { return true; }
    bool operator==(const Foo&) const { return true; }
};
template<>
constexpr bool jpt::IsTrivial<Foo> = true;

bool UnitTests_MinMax()
{
    JPT_ENSURE(jpt::Min(1, 5) == 1);
    JPT_ENSURE(jpt::Min(2, 5, 10) == 2);
    JPT_ENSURE(jpt::Min(5, 1, 2, 5) == 1);
    JPT_ENSURE(jpt::Max(1, 5) == 5);
    JPT_ENSURE(jpt::Max(2, 5, 10) == 10);
    JPT_ENSURE(jpt::Max(5, 1, 2, 5) == 5);

    Foo f1, f2;
    JPT_ENSURE(jpt::Max(f1, f2) == f1);

    return true;
}

bool UnitTests_Floor()
{
    JPT_ENSURE(jpt::Floor(0.0) == 0);

    JPT_ENSURE(jpt::Floor(1.0) == 1);
    JPT_ENSURE(jpt::Floor(1.3) == 1);
    JPT_ENSURE(jpt::Floor(1.5) == 1);
    JPT_ENSURE(jpt::Floor(1.7) == 1);
    JPT_ENSURE(jpt::Floor(1.3f) == 1);
    JPT_ENSURE(jpt::Floor(1.5f) == 1);
    JPT_ENSURE(jpt::Floor(1.7f) == 1);

    JPT_ENSURE(jpt::Floor(-1.0) == -1);
    JPT_ENSURE(jpt::Floor(-1.3) == -2);
    JPT_ENSURE(jpt::Floor(-1.5) == -2);
    JPT_ENSURE(jpt::Floor(-1.7) == -2);
    JPT_ENSURE(jpt::Floor(-1.3f) == -2);
    JPT_ENSURE(jpt::Floor(-1.5f) == -2);
    JPT_ENSURE(jpt::Floor(-1.7f) == -2);

    return true;
}

bool UnitTests_Ceil()
{
    JPT_ENSURE(jpt::Ceil(0.0) == 0);

    JPT_ENSURE(jpt::Ceil(1.0) == 1);
    JPT_ENSURE(jpt::Ceil(1.3) == 2);
    JPT_ENSURE(jpt::Ceil(1.5) == 2);
    JPT_ENSURE(jpt::Ceil(1.7) == 2);
    JPT_ENSURE(jpt::Ceil(1.3f) == 2);
    JPT_ENSURE(jpt::Ceil(1.5f) == 2);
    JPT_ENSURE(jpt::Ceil(1.7f) == 2);

    JPT_ENSURE(jpt::Ceil(-1.3) == -1);
    JPT_ENSURE(jpt::Ceil(-1.5) == -1);
    JPT_ENSURE(jpt::Ceil(-1.7) == -1);
    JPT_ENSURE(jpt::Ceil(-1.3f) == -1);
    JPT_ENSURE(jpt::Ceil(-1.5f) == -1);
    JPT_ENSURE(jpt::Ceil(-1.7f) == -1);

    return true;
}

bool UnitTests_Round()
{
    JPT_ENSURE(jpt::Round(0.0) == 0);

    JPT_ENSURE(jpt::Round(1.0) == 1);
    JPT_ENSURE(jpt::Round(1.3) == 1);
    JPT_ENSURE(jpt::Round(1.5) == 2);
    JPT_ENSURE(jpt::Round(1.7) == 2);
    JPT_ENSURE(jpt::Round(1.3f) == 1);
    JPT_ENSURE(jpt::Round(1.5f) == 2);
    JPT_ENSURE(jpt::Round(1.7f) == 2);

    JPT_ENSURE(jpt::Round(-1.3) == -1);
    JPT_ENSURE(jpt::Round(-1.5) == -1);
    JPT_ENSURE(jpt::Round(-1.7) == -2);
    JPT_ENSURE(jpt::Round(-1.3f) == -1);
    JPT_ENSURE(jpt::Round(-1.5f) == -1);
    JPT_ENSURE(jpt::Round(-1.7f) == -2);

    return true;
}

bool UnitTests_FloorCeil()
{
    JPT_ENSURE(jpt::FloorCeil(0.0) == 0);

    JPT_ENSURE(jpt::FloorCeil(-1.0) == -1);
    JPT_ENSURE(jpt::FloorCeil(-0.8) == -1);
    JPT_ENSURE(jpt::FloorCeil(-0.7) == -1);
    JPT_ENSURE(jpt::FloorCeil(-0.5) == -1);
    JPT_ENSURE(jpt::FloorCeil(-0.3) == -1);

    JPT_ENSURE(jpt::FloorCeil(0.3) == 1);
    JPT_ENSURE(jpt::FloorCeil(0.5) == 1);
    JPT_ENSURE(jpt::FloorCeil(0.7) == 1);
    JPT_ENSURE(jpt::FloorCeil(0.8) == 1);
    JPT_ENSURE(jpt::FloorCeil(1.0) == 1);

    return true;
}

export bool RunUnitTests_Math()
{
    JPT_ENSURE(UnitTests_Clamping());
    JPT_ENSURE(UnitTests_Interpolation());
    JPT_ENSURE(UnitTests_Abs());
    JPT_ENSURE(UnitTests_AreValuesClose());
    JPT_ENSURE(UnitTests_MinMax());

    JPT_ENSURE(UnitTests_Floor());
    JPT_ENSURE(UnitTests_Ceil());
    JPT_ENSURE(UnitTests_Round());
    JPT_ENSURE(UnitTests_FloorCeil());

    return true;
}