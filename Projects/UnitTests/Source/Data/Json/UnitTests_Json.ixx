// Copyright Jupiter Technologies, Inc. All Rights Reserved.

module;

#include "Core/Minimal/Headers.h"

export module UnitTests_Json;

import jpt.TypeDefs;
import jpt.Utilities;
import jpt.Math;

import jpt.JsonUtils;
import jpt.JsonObject;

import jpt.File.Path;
import jpt.FileIO;

bool UnitTest_Json_Read()
{
    const jpt::File::Path path = { jpt::File::ESource::Engine, "Assets/Configs/TestJson.json" };

    // Reads a json file. All the data should have been read into memory with this call
    jpt::JsonObject jsonRoot = jpt::ReadJsonRoot(path).Value();

    // Access the data with Get<T>() 
    JPT_ENSURE(jsonRoot.Get<jpt::String>("source") == "Engine");
    JPT_ENSURE(jsonRoot.Get<int32>("data_int") == 12);
    JPT_ENSURE(jpt::AreValuesClose(jsonRoot.Get<float32>("data_float"), 1.55));
    JPT_ENSURE(jsonRoot.Get<jpt::String>("data_string") == "Hello from Jupiter Engine!");
    JPT_ENSURE(jsonRoot.Get<jpt::String>("data_null") == "Invalid");

    return true;
}

export bool RunUnitTests_Json()
{
    JPT_ENSURE(UnitTest_Json_Read());

    return true;
}
