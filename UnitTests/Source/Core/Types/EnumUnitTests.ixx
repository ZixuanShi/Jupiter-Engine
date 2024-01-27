// Copyright Jupiter Technologies, Inc. All Rights Reserved.

module;

#include "Core/Minimals/Headers.h"

export module UnitTests_Enum;

import jpt.CoreModules;

bool UnitTest_CreateEnum()
{
	//enum class EFruit
	//{
	//	kApple,
	//	kOrange,
	//	kPeach,
	//};

	//class EnumElement
	//{
	//	int32 value;
	//	const char* str = "";
	//};

	//class JptEnum
	//{
	//private: 
	//	StaticArray<EnumElement> m_enumElements;

	//public:
	// constexpr JptEnum()
	// 
	//	ToString();
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

export bool RunUnitTests_Enum()
{
	JPT_RETURN_FALSE_IF_ERROR(!UnitTest_CreateEnum(), "");

	return true;
}