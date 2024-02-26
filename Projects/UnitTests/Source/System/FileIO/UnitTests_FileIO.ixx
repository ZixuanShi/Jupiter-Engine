// Copyright Jupiter Technologies, Inc. All Rights Reserved.

module;

#include "Core/Minimal/Headers.h"

export module UnitTests_FileIO;

import jpt.FileIO;
import jpt.TypeDefs;
import jpt.Utilities;

bool UnitTest_FileIO()
{
    jpt::FileIO::Read("Engine:Configs/TestJson.json");
    jpt::FileIO::Read("Engine:Configs/TestJson.json");

    jpt::FileIO::Read("Client:Configs/TestJson.json");
    jpt::FileIO::Read("Client:Configs/TestJson.json");

    return true;
}

export bool RunUnitTests_FileIO()
{
    JPT_ENSURE(UnitTest_FileIO());

    return true;
}
