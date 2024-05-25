// Copyright Jupiter Technologies, Inc. All Rights Reserved.

module;

#include "Core/Minimal/Headers.h"

export module UnitTests_FileIO;

import jpt.File_Base;
import jpt.File_Text;
import jpt.FileIO;
import jpt.TypeDefs;
import jpt.Utilities;

bool UnitTest_FileIO_Read_Engine_English()
{

    return true;
}

export bool RunUnitTests_FileIO()
{
    JPT_ENSURE(UnitTest_FileIO_Read_Engine_English());

    return true;
}
