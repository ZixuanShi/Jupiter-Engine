// Copyright Jupiter Technologies, Inc. All Rights Reserved.

module;

#include "Core/Minimal/CoreHeaders.h"

export module UnitTests_BoolPack;

import jpt.BoolPack;

bool UnitTests_BoolPack()
{
    jpt::BoolPack pack;
    pack.Set(0, true);
    pack.Set(1, false);
    pack.Set(2, true);
    pack.Set(3, true);
    pack.Set(4, false);
    pack.Set(5, false);
    pack.Set(6, true);
    pack.Set(7, false);

    JPT_ENSURE(pack.Get(0) == true);
    JPT_ENSURE(pack.Get(1) == false);
    JPT_ENSURE(pack.Get(2) == true);
    JPT_ENSURE(pack.Get(3) == true);
    JPT_ENSURE(pack.Get(4) == false);
    JPT_ENSURE(pack.Get(5) == false);
    JPT_ENSURE(pack.Get(6) == true);
    JPT_ENSURE(pack.Get(7) == false);
    JPT_ENSURE(pack[0] == true);
    JPT_ENSURE(pack[1] == false);
    JPT_ENSURE(pack[2] == true);
    JPT_ENSURE(pack[3] == true);
    JPT_ENSURE(pack[4] == false);
    JPT_ENSURE(pack[5] == false);
    JPT_ENSURE(pack[6] == true);
    JPT_ENSURE(pack[7] == false);

    return true;
}

export bool RunUnitTests_BoolPack()
{
    JPT_ENSURE(UnitTests_BoolPack());

    return true;
}