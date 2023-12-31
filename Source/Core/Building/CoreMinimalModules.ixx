// Copyright Jupiter Technologies, Inc. All Rights Reserved.

module;

#include <iostream>
#include <Windows.h>
#include <string>

export module jpt.CoreMinimalModules;

namespace jpt
{
	export struct Foo
	{
		static void PrintNumber();
		void PrintMemberData();

		int m_data = 10;
	};

	void Foo::PrintNumber()
	{
		::OutputDebugStringA((std::to_string(42) + "\n").c_str());
		std::cout << 42 << std::endl;
	}

	void Foo::PrintMemberData()
	{
		::OutputDebugStringA((std::to_string(m_data) + "\n").c_str());
		std::cout << m_data << std::endl;
	}
}
