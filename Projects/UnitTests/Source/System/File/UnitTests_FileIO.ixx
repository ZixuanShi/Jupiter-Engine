// Copyright Jupiter Technologies, Inc. All Rights Reserved.

module;

#include "Core/Minimal/Headers.h"

export module UnitTests_FileIO;

import jpt.FileIO;
import jpt.TypeDefs;
import jpt.Utilities;

bool UnitTest_FileIO_Read()
{
    //jpt::FileIO::Read(jpt::ESource::Engine, "Assets/Configs/TestJson.json");
    //jpt::FileIO::Read("C:/Program Files/Jupiter Technologies/Jupiter-Engine/Assets/Configs/TestJson.json");

    //jpt::FileIO::Read(jpt::ESource::Client, "Assets/Configs/TestJson.json");
    //jpt::FileIO::Read("C:/Program Files/Jupiter Technologies/Jupiter-Engine/Projects/UnitTests/Assets/Configs/TestJson.json");

    return true;
}

export bool RunUnitTests_FileIO()
{
    JPT_ENSURE(UnitTest_FileIO_Read());

    return true;
}
