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

static const jpt::File::Path path = { jpt::File::ESource::Client, "Assets/TestJson_UnitTest.json" };

bool UnitTest_Json_BasicWrite()
{
    jpt::JsonObject jsonRoot;

    jsonRoot.Add("source", jpt::String("Engine"));
    jsonRoot.Add("data_int", 12);
    jsonRoot.Add("data_float", 1.55f);
    jsonRoot.Add("data_string", jpt::String("Hello from Jupiter Engine!"));
    jsonRoot.Add("data_null", jpt::String("null"));

    jsonRoot.Add("data_array", jpt::JsonArray{111, jpt::String("Hello World"), 33, 4.77f, false});

    jsonRoot.Add("data_map", jpt::JsonMap
        {
            { "year",      2024 },
            { "month",     6    },
            { "day",       20   },
            { "Calender", jpt::String("Made in China")}
        });

    jpt::WriteJsonRoot(path, jsonRoot);

    return true;
}

bool UnitTest_Json_BasicRead()
{
    // Reads a json file. All the data should have been read into memory with this call
    jpt::JsonObject jsonRoot = jpt::ReadJsonRoot(path).Value();

    // Access the data with Get<T>() 
    JPT_ENSURE(jsonRoot.Get<jpt::String>("source") == "Engine");
    JPT_ENSURE(jsonRoot.Get<int32>("data_int") == 12);
    JPT_ENSURE(jpt::AreValuesClose(jsonRoot.Get<float32>("data_float"), 1.55));
    JPT_ENSURE(jsonRoot.Get<jpt::String>("data_string") == "Hello from Jupiter Engine!");
    JPT_ENSURE(jsonRoot.Get<jpt::String>("data_null") == "null");

    jpt::JsonArray data_array = jsonRoot.Get<jpt::JsonArray>("data_array");
    jpt::JsonArray expected_data_array{ 111, jpt::String("Hello World"), 33, 4.77f, false};
    JPT_ENSURE(data_array == expected_data_array);

    // Subset map
    const jpt::JsonMap& subSet = jsonRoot.Get<jpt::JsonMap>("data_map");
    JPT_ENSURE(subSet["year"] == 2024);
    JPT_ENSURE(subSet["month"] == 6);
    JPT_ENSURE(subSet["day"] == 20);
    JPT_ENSURE(subSet["Calender"] == jpt::String("Made in China"));

    return true;
}

export bool RunUnitTests_Json()
{
    JPT_ENSURE(UnitTest_Json_BasicWrite());
    JPT_ENSURE(UnitTest_Json_BasicRead());

    return true;
}
