// Copyright Jupiter Technologies, Inc. All Rights Reserved.

module;

#include "Core/Minimals/Headers.h"

export module TupleUnitTests;

import jpt.CoreModules;

bool UnitTest_Tuple()
{
	class Foo
	{
	public:
		jpt::String ToString() const
		{
			return "I am Foo";
		}
	};

	//jpt::Tuple<int, char, bool, Foo> tuple1;
	//jpt::Tuple<int, char, bool, Foo> tuple2{3, 'C', false, Foo()};
	
	//JPT_LOG(jpt::get<0>(tuple));
	//JPT_LOG(jpt::get<1>(tuple));
	//JPT_LOG(jpt::get<2>(tuple));
	//JPT_LOG(jpt::get<3>(tuple));

	return true;
}

export bool RunTupleUnitTests()
{
	JPT_RETURN_FALSE_IF_ERROR(!UnitTest_Tuple(), "UnitTest_Tuple Failed");

	return true;
}