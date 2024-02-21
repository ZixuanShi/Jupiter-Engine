// Copyright Jupiter Technologies, Inc. All Rights Reserved.

module;

#include "Core/Minimal/Headers.h"

export module UnitTests_Function;

import jpt.Function;
import jpt.TypeDefs;
import jpt.Utilities;

bool TestFunction1(int32& n)
{
    if (n < 10)
    {
        return true;
    }

    n = 100;
    return false;
}

namespace jpt_private
{
    int32 TestFunction2(int32 n)
    {
        return n * 2;
    }
}

template<jpt::Functoring T>
void TestHasSize(T&& t)
{
    t(5);
}

bool UnitTest_Function()
{
    // Global function
    jpt::Function<bool(int32&)> function = TestFunction1;
    int32 n = 5;
    JPT_ENSURE(function(n));
    JPT_ENSURE(n == 5);

    n = 10;
    JPT_ENSURE(!function(n));
	JPT_ENSURE(n == 100);

    jpt::Function<bool(int32)> function2 = [](int32 n) { JPT_ENSURE(n == 5); return true; };
    function2(5);

    // Class member function
    struct Foo
    {
        jpt::Function<int32(int32)> m_function;

        int32 Work(int32 n)
		{
            JPT_LOG("Foo::Work()");
			return m_function(n);
		}

        void operator()(int n)
        {
			JPT_LOG("Foo::operator()");
			JPT_LOG(n);
        }
    };

    Foo foo;
    foo.m_function = jpt_private::TestFunction2;
    JPT_ENSURE(foo.Work(5) == 10);

    TestHasSize(foo);

    return true;
}

bool UnitTest_Lambda()
{
    auto lambda = [](int32& n) -> bool
        {
            if (n < 10)
            {
                return true;
            }

            n = 100;
            return false;
        };
    jpt::Function<bool(int32&)> function = lambda;
    int32 n = 5;
    JPT_ENSURE(function(n));
    JPT_ENSURE(n == 5);

    n = 10;
    JPT_ENSURE(!function(n));
    JPT_ENSURE(n == 100);

    return true;
}

export bool RunUnitTests_Function()
{  

    JPT_ENSURE(UnitTest_Function());
    JPT_ENSURE(UnitTest_Lambda());

    return true;
}
