// Copyright Jupiter Technologies, Inc. All Rights Reserved.

module;

#include "Core/Minimal/CoreHeaders.h"

export module UnitTests_Frameworks;

import jpt.TypeDefs;
import jpt.Utilities;

import UnitTests_ECS;
import UnitTests_EventSystem;

export bool RunUnitTests_Frameworks()
{
    JPT_ENSURE(RunUnitTests_ECS());
    JPT_ENSURE(RunUnitTests_EventSystem());

    return true;
}