// Copyright Jupiter Technologies, Inc. All Rights Reserved.

module;

#include "Core/Minimal/Headers.h"

export module UnitTests_Enum;

import jpt.Enum;
import jpt.TypeDefs;

//jpt::Enum EFruits
//{
//	"Apple",
//	//"Banana",
//	//"Orange",
//	//
//	//"End"
//};

bool UnitTest_Enum()
{

	return true;
}

export bool RunUnitTests_Enum()
{
	JPT_RETURN_FALSE_IF_ERROR(!UnitTest_Enum(), "");

	//jpt::Enum EWeapon
	//{
	//	"Sword",
	//	//"Axe",
	//	//"Spear",

	//	//"End"
	//};

	return true;
}
