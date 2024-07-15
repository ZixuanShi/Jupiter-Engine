// Copyright Jupiter Technologies, Inc. All Rights Reserved.

module;

#include "Core/Minimal/CoreHeaders.h"

export module UnitTests_Comparators;

import jpt.Comparators;
import jpt.String;
import jpt.TypeDefs;
import jpt.Utilities;

bool UnitTest_Comparators_Less()
{
    jpt::Comparator_Less<int32> intComparator;
    JPT_ENSURE(intComparator(1, 2));
    JPT_ENSURE(!intComparator(2, 1));

    jpt::Comparator_Less<char> charComparator;
    JPT_ENSURE(charComparator('a', 'b'));
    JPT_ENSURE(!charComparator('b', 'a'));

    return true;
}

bool UnitTest_Comparators_Greater()
{
    jpt::Comparator_Greater<int32> intComparator;
    JPT_ENSURE(intComparator(2, 1));
    JPT_ENSURE(!intComparator(1, 2));

    jpt::Comparator_Greater<char> charComparator;
    JPT_ENSURE(charComparator('b', 'a'));
    JPT_ENSURE(!charComparator('a', 'b'));

    return true;
}

bool UnitTest_Comparators_Equal()
{
    // int
    jpt::Comparator_Equal<int32> intComparator;
    JPT_ENSURE(intComparator(1, 1));
    JPT_ENSURE(!intComparator(1, 2));

    // string
    jpt::Comparator_Equal<jpt::String> strComparator;
    JPT_ENSURE(strComparator(jpt::String("Hello"), jpt::String("Hello")));
    JPT_ENSURE(!strComparator(jpt::String("Hello"), jpt::String("World")));

    // cstr
    jpt::Comparator_Equal<const char*> cstrComparator;
    JPT_ENSURE(cstrComparator("Hello", "Hello"));
    JPT_ENSURE(!cstrComparator("Hello", "World"));

    // Object
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

    // void. Should work for any type.
    jpt::Comparator_Equal<void> voidComparator;
    JPT_ENSURE(voidComparator(nullptr, nullptr));
    JPT_ENSURE(!voidComparator(nullptr, &foo1));
    JPT_ENSURE(!voidComparator(&foo1, nullptr));
    JPT_ENSURE(voidComparator(&foo1, &foo1));
    JPT_ENSURE(!voidComparator(&foo1, &foo2));
    JPT_ENSURE(voidComparator(1, 1));
    JPT_ENSURE(!voidComparator(1, 2));
    JPT_ENSURE(voidComparator(jpt::String("Hello"), jpt::String("Hello")));
    JPT_ENSURE(!voidComparator(jpt::String("Hello"), jpt::String("World")));
    //JPT_ENSURE(voidComparator("Hello", "Hello"));
    //JPT_ENSURE(!voidComparator("Hello", "World"));

    // Non-void but wrong parameter type. This should not compile.
    //jpt::Comparator_Equal<char> badComparator;
    //badComparator(nullptr, nullptr);

    return true;
}

export bool RunUnitTests_Comparators()
{
    JPT_ENSURE(UnitTest_Comparators_Less());
    JPT_ENSURE(UnitTest_Comparators_Greater());
    JPT_ENSURE(UnitTest_Comparators_Equal());

    return true;
}
