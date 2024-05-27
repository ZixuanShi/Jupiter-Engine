// Copyright Jupiter Technologies, Inc. All Rights Reserved.

module;

#include "Core/Minimal/Headers.h"

export module UnitTests_Comparators;

import jpt.Comparators;
import jpt.String;
import jpt.TypeDefs;
import jpt.Utilities;

bool UnitTest_Comparators_Equal()
{
    jpt::Comparator_Equal<int32> intComparator;
    JPT_ENSURE(intComparator(1, 1));
    JPT_ENSURE(!intComparator(1, 2));

    jpt::Comparator_Equal<jpt::String> strComparator;
    JPT_ENSURE(strComparator(jpt::String("Hello"), jpt::String("Hello")));
    JPT_ENSURE(!strComparator(jpt::String("Hello"), jpt::String("World")));

    jpt::Comparator_Equal<const char*> cstrComparator;
    JPT_ENSURE(cstrComparator("Hello", "Hello"));
    JPT_ENSURE(!cstrComparator("Hello", "World"));

    struct Foo
    {
        int32 data = 10;

        bool operator==(Foo other) const
		{
			return data == other.data;
		}
    };

    Foo foo1;
    Foo foo2(0);
    jpt::Comparator_Equal<Foo> fooComparator;
    JPT_ENSURE(fooComparator(foo1, foo1));
    JPT_ENSURE(!fooComparator(foo1, foo2));

    return true;
}

export bool RunUnitTests_Comparators()
{
    JPT_ENSURE(UnitTest_Comparators_Equal());

    return true;
}
