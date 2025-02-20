// Copyright Jupiter Technologies, Inc. All Rights Reserved.

module;

#include "Core/Minimal/CoreHeaders.h"

export module UnitTests_Math;

import jpt.TypeDefs;
import jpt.TypeTraits;
import jpt.Math;
import jpt.Utilities;
import jpt.Sort;

bool UnitTests_Clamping()
{
	int32 n = jpt::Clamp(5, 10, 13);
	JPT_ENSURE(n == 10);

	jpt::ClampTo(n, 5, 13);
	JPT_ENSURE(n == 10);

	jpt::ClampTo(n, 1, 3);
	JPT_ENSURE(n == 3);

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
	JPT_ENSURE(jpt::Floor(1.3) == 1);
	JPT_ENSURE(jpt::Floor(1.5) == 1);
	JPT_ENSURE(jpt::Floor(1.7) == 1);
	JPT_ENSURE(jpt::Floor(1.3f) == 1);
	JPT_ENSURE(jpt::Floor(1.5f) == 1);
	JPT_ENSURE(jpt::Floor(1.7f) == 1);

	return true;
}

bool UnitTests_FloorCeil()
{
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
	JPT_ENSURE(UnitTests_Abs());
	JPT_ENSURE(UnitTests_AreValuesClose());
	JPT_ENSURE(UnitTests_MinMax());
	JPT_ENSURE(UnitTests_Floor());
	JPT_ENSURE(UnitTests_FloorCeil());

	return true;
}