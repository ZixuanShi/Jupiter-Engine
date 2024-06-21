// Copyright Jupiter Technologies, Inc. All Rights Reserved.

module;

#include "Core/Minimal/Headers.h"

export module UnitTests_Json;

import jpt.TypeDefs;
import jpt.Utilities;

import jpt.File.Path;
import jpt.FileIO;

bool UnitTest_Json_Read()
{
    //const jpt::File::Path path = { jpt::File::ESource::Engine, "Assets/Configs/TestJson.json" };
    
    //jpt::Json::Read(path);

    return true;
}

export bool RunUnitTests_Json()
{
    JPT_ENSURE(UnitTest_Json_Read());

    return true;
}
