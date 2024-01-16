// Copyright Jupiter Technologies, Inc. All Rights Reserved.

module;

#include "Core/Minimals/Headers.h"

export module EnumUnitTests;

import jpt.CoreModules;

bool UnitTest_CreateEnum()
{
	//enum class EFruit
	//{
	//	kApple,
	//	kOrange,
	//	kPeach,
	//};
	//EFruit::kApple;
	//EFruit::kOrange;

	//jpt::Enum EFruit
	//{
	//	"Apple",
	//	"Orange",
	//	"Peach",
	//};
	//EFruit.Val("Apple");

	return true;
}

export bool RunEnumUnitTests()
{
	JPT_RETURN_FALSE_IF_ERROR(!UnitTest_CreateEnum(), "");

	return true;
}