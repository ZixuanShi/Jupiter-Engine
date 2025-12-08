// Copyright Jupiter Technologies, Inc. All Rights Reserved.

module;

#include "Core/Minimal/CoreHeaders.h"

export module UnitTests_Logger;

import jpt.ToString;
import jpt.String;
import jpt.Utilities;

class Foo
{
public:
};

namespace jpt
{
    String ToString(const Foo&)
    {
        return jpt::String("I am a Foo object");
    }
}

bool UnitTests_Logger_Data()
{
    JPT_INFO("%d", 1);
    JPT_INFO(1);

    JPT_INFO(2);
    JPT_INFO(2.0f);
    JPT_INFO(2.0);
    JPT_INFO(false);
    JPT_INFO(true);
    JPT_INFO('C');
    JPT_INFO('a');
    JPT_INFO("I'm genius");

    Foo foo;
    JPT_INFO(foo);
    JPT_INFO(jpt::ToString(foo));
    JPT_INFO(jpt::ToString(foo).ConstBuffer());
    JPT_INFO("%s", jpt::ToString(foo).ConstBuffer());

    JPT_ENSURE(jpt::ToString(foo) == "I am a Foo object");

    return true;
}

export bool RunUnitTests_Logger()
{
    //JPT_ENSURE(UnitTests_Logger_Data());

    return true;
}