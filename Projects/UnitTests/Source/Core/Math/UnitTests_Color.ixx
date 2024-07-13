// Copyright Jupiter Technologies, Inc. All Rights Reserved.

module;

#include "Core/Minimal/Headers.h"

export module UnitTests_Color;

import jpt.Color;
import jpt.Math;
import jpt.TypeDefs;
import jpt.Utilities;

static bool UnitTest_Color()
{
    Color color(0xFF00FFFF);
    JPT_ENSURE(color.r == 0xFF);
    JPT_ENSURE(color.g == 0x00);
    JPT_ENSURE(color.b == 0xFF);
    JPT_ENSURE(color.a == 0xFF);
    JPT_ENSURE(color.RGBA() == 0xFF00FFFF);
    JPT_ENSURE(color == Color::Magenta);

    color.r = 0x00;
    JPT_ENSURE(color.r == 0x00);
    JPT_ENSURE(color.RGBA() == 0x0000FFFF);
    JPT_ENSURE(color == Color::Blue);

    return true;
}

static bool UnitTest_Color_Lerp()
{
    Color color1(0x000000FF);
	Color color2(0x00FF00FF);
	Color color3 = Lerp(color1, color2, 0.5f);
    JPT_ENSURE(color3 == Color(0x007F00FF));

	return true;
}

export bool RunUnitTests_Color()
{
    JPT_ENSURE(UnitTest_Color());
    JPT_ENSURE(UnitTest_Color_Lerp());

    return true;
}
