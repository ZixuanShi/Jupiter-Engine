// Copyright Jupiter Technologies, Inc. All Rights Reserved.

module;

#include "Core/Minimal/Headers.h"

export module UnitTests_Json;

import jpt.TypeDefs;
import jpt.Utilities;
import jpt.Math;

import jpt.JsonUtils;
import jpt.JsonData;

import jpt.File.Path;
import jpt.FileIO;

static const jpt::File::Path path = { jpt::File::ESource::Client, "Assets/TestJson_UnitTest.json" };

bool UnitTest_Json_Write()
{
    jpt::JsonMap jsonRoot;

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
            { "Calender", jpt::String("Made in China")},

            { "Subset", jpt::JsonMap 
                        {
            				{ "key1", 1 },
							{ "key2", 2 },
							{ "key3", 3 }
                        }}
        });

    jpt::WriteJsonRoot(path, jsonRoot);

    return true;
}

bool UnitTest_Json_Read()
{
    // Reads a json file. All the data should have been read into memory with this call
    jpt::JsonMap jsonRoot = jpt::ReadJsonRoot(path).Value();

    // Access the data with Get<T>() 
    JPT_ENSURE(jsonRoot["source"] == jpt::String("Engine"));
    JPT_ENSURE(jsonRoot["data_int"] == 12);
    JPT_ENSURE(jpt::AreValuesClose(jsonRoot["data_float"].As<float32>(), 1.55f));
    JPT_ENSURE(jsonRoot["data_string"] == jpt::String("Hello from Jupiter Engine!"));
    JPT_ENSURE(jsonRoot["data_null"] == jpt::String("null"));

    jpt::JsonArray data_array = jsonRoot["data_array"].As<jpt::JsonArray>();
    jpt::JsonArray expected_data_array{ 111, jpt::String("Hello World"), 33, 4.77f, false};
    JPT_ENSURE(data_array == expected_data_array);

    // Subset map
    const jpt::JsonMap& subSet = jsonRoot["data_map"].As<jpt::JsonMap>();
    JPT_ENSURE(subSet["year"] == 2024);
    JPT_ENSURE(subSet["month"] == 6);
    JPT_ENSURE(subSet["day"] == 20);
    JPT_ENSURE(subSet["Calender"] == jpt::String("Made in China"));

    const jpt::JsonMap& subSet2 = subSet["Subset"].As<jpt::JsonMap>();
    JPT_ENSURE(subSet2["key1"] == 1);
    JPT_ENSURE(subSet2["key2"] == 2);
    JPT_ENSURE(subSet2["key3"] == 3);

    return true;
}

export bool RunUnitTests_Json()
{
    JPT_ENSURE(UnitTest_Json_Write());
    JPT_ENSURE(UnitTest_Json_Read());

    return true;
}
