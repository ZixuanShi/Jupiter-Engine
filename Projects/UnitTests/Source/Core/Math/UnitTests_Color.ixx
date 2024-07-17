// Copyright Jupiter Technologies, Inc. All Rights Reserved.

module;

#include "Core/Minimal/CoreHeaders.h"

export module UnitTests_Color;

import jpt.Color;
import jpt.LinearColor;
import jpt.Math;
import jpt.TypeDefs;
import jpt.Utilities;

static bool UnitTests_Color()
{
    Color color(0xFF00FFFF);
    JPT_ENSURE(color.r == 0xFF);
    JPT_ENSURE(color.g == 0x00);
    JPT_ENSURE(color.b == 0xFF);
    JPT_ENSURE(color.a == 0xFF);
    JPT_ENSURE(color.ToRGBA() == 0xFF00FFFF);
    JPT_ENSURE(color == Color::Magenta());

    color.r = 0x00;
    JPT_ENSURE(color.r == 0x00);
    JPT_ENSURE(color.ToRGBA() == 0x0000FFFF);
    JPT_ENSURE(color == Color::Blue());

    return true;
}

static bool UnitTests_Color_Lerp()
{
    Color color1(0x000000FF);
	Color color2(0x00FF00FF);
	Color color3 = Lerp(color1, color2, 0.5f);
    JPT_ENSURE(color3 == Color(0x007F00FF));

	return true;
}

static bool UnitTests_Color_FromRGBA()
{
    Color color;

    color.FromRGBA(0xFFFFFFFF);
    JPT_ENSURE(color.r == 0xFF);
    JPT_ENSURE(color.g == 0xFF);
    JPT_ENSURE(color.b == 0xFF);
    JPT_ENSURE(color.a == 0xFF);
    JPT_ENSURE(color.ToRGBA() == 0xFFFFFFFF);

    color.FromRGBA(0x000000FF);
    JPT_ENSURE(color.r == 0x00);
    JPT_ENSURE(color.g == 0x00);
    JPT_ENSURE(color.b == 0x00);
    JPT_ENSURE(color.a == 0xFF);
    JPT_ENSURE(color.ToRGBA() == 0x000000FF);

    return true;
}

static bool UnitTests_Color_FromLinearColor()
{
    Color color;

    color.FromLinearColor(LinearColor(1.0f, 1.0f, 1.0f));
    JPT_ENSURE(color.r == 0xFF);
    JPT_ENSURE(color.g == 0xFF);
    JPT_ENSURE(color.b == 0xFF);
    JPT_ENSURE(color.a == 0xFF);
    JPT_ENSURE(color.ToRGBA() == 0xFFFFFFFF);

    color.FromLinearColor(LinearColor(0.0f, 0.0f, 0.0f));
    JPT_ENSURE(color.r == 0x00);
    JPT_ENSURE(color.g == 0x00);
    JPT_ENSURE(color.b == 0x00);
    JPT_ENSURE(color.a == 0xFF);
    JPT_ENSURE(color.ToRGBA() == 0x000000FF);

    return true;
}

static bool UnitTests_LinearColor()
{
	LinearColor linearColor(1.0f, 0.0f, 1.0f);
	JPT_ENSURE(linearColor.r == 1.0f);
	JPT_ENSURE(linearColor.g == 0.0f);
	JPT_ENSURE(linearColor.b == 1.0f);
	JPT_ENSURE(linearColor.a == 1.0f);
	JPT_ENSURE(linearColor.ToRGBA() == 0xFF00FFFF);
	JPT_ENSURE(linearColor == LinearColor::Magenta());

	linearColor.r = 0.0f;
	JPT_ENSURE(linearColor.r == 0.0f);
	JPT_ENSURE(linearColor.ToRGBA() == 0x0000FFFF);
	JPT_ENSURE(linearColor == LinearColor::Blue());

	return true;
}

static bool UnitTests_LinearColor_Lerp()
{
	LinearColor linearColor1(0.0f, 0.0f, 0.0f);
	LinearColor linearColor2(1.0f, 1.0f, 1.0f);
	LinearColor linearColor3 = Lerp(linearColor1, linearColor2, 0.5f);
	JPT_ENSURE(linearColor3 == LinearColor(0.5f, 0.5f, 0.5f));
    JPT_ENSURE(linearColor3.ToRGBA() == 0x7F7F7FFF);
    JPT_ENSURE(linearColor3 == LinearColor::Gray());

	return true;
}

static bool UnitTests_LinearColor_FromRGBA()
{
	LinearColor linearColor;

	linearColor.FromRGBA(0xFFFFFFFF);
	JPT_ENSURE(linearColor.r == 1.0f);
	JPT_ENSURE(linearColor.g == 1.0f);
	JPT_ENSURE(linearColor.b == 1.0f);
	JPT_ENSURE(linearColor.a == 1.0f);
	JPT_ENSURE(linearColor.ToRGBA() == 0xFFFFFFFF);
    JPT_ENSURE(linearColor == LinearColor::White());

	linearColor.FromRGBA(0x000000FF);
	JPT_ENSURE(linearColor.r == 0.0f);
	JPT_ENSURE(linearColor.g == 0.0f);
	JPT_ENSURE(linearColor.b == 0.0f);
	JPT_ENSURE(linearColor.a == 1.0f);
	JPT_ENSURE(linearColor.ToRGBA() == 0x000000FF);
    JPT_ENSURE(linearColor == LinearColor::Black());

	return true;
}

export bool RunUnitTests_Color()
{
    JPT_ENSURE(UnitTests_Color());
    JPT_ENSURE(UnitTests_Color_Lerp());
    JPT_ENSURE(UnitTests_Color_FromRGBA());
    JPT_ENSURE(UnitTests_Color_FromLinearColor());

    JPT_ENSURE(UnitTests_LinearColor());
    JPT_ENSURE(UnitTests_LinearColor_Lerp());
    JPT_ENSURE(UnitTests_LinearColor_FromRGBA());

    return true;
}
