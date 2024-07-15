// Copyright Jupiter Technologies, Inc. All Rights Reserved.

module;

#include "Core/Minimal/CoreHeaders.h"

export module UnitTests_Json;

import jpt.TypeDefs;
import jpt.Utilities;
import jpt.Math;

import jpt.JsonUtils;
import jpt.JsonData;

import jpt.File.Path;
import jpt.FileIO;

static const jpt::File::Path path = { jpt::File::ESource::Client, "Assets/TestJson_UnitTest.json" };
static const jpt::File::Path engineJsonPath = { jpt::File::ESource::Engine, "Assets/TestJson_UnitTest.json" };

bool UnitTest_Json_Write()
{
    jpt::JsonMap jsonRoot;

    jsonRoot.Set("source", jpt::String("Client"));
    jsonRoot.Set("data_int", 12);
    jsonRoot.Set("data_float", 1.55f);
    jsonRoot.Set("data_string", jpt::String("Hello from Jupiter Engine!"));
    jsonRoot.Set("data_null", jpt::String("null"));

    jsonRoot.Set("data_array", jpt::JsonArray{111, jpt::String("Hello World"), 33, 4.77f, false});

    jpt::JsonMap subMap2;
    subMap2.Set("key1", 1);
    subMap2.Set("key2", 2);
    subMap2.Set("key3", 3);

    jpt::JsonMap subMap;
    subMap.Set("year", 2024);
    subMap.Set("month", 6);
    subMap.Set("day", 20);
    subMap.Set("Subset", subMap2);

    jsonRoot.Set("data_map", subMap);

    jpt::WriteJsonFile(path, jsonRoot);

    return true;
}

bool UnitTest_Json_Read()
{
    // Reads a json file. All the data should have been read into memory with this call
    jpt::JsonMap jsonRoot = jpt::ReadJsonFile(path).Value();

    // Access the data with operator[]
    JPT_ENSURE(jsonRoot["source"] == jpt::String("Client"));
    JPT_ENSURE(jsonRoot["data_int"] == 12);
    JPT_ENSURE(jpt::AreValuesClose(jsonRoot["data_float"].As<float32>(), 1.55f));
    JPT_ENSURE(jsonRoot["data_string"] == jpt::String("Hello from Jupiter Engine!"));
    JPT_ENSURE(jsonRoot["data_null"] == jpt::String("null"));

    const jpt::JsonArray& data_array = jsonRoot["data_array"].As<jpt::JsonArray>();
    jpt::JsonArray expected_data_array{ 111, jpt::String("Hello World"), 33, 4.77f, false};
    JPT_ENSURE(data_array == expected_data_array);

    // Subset map
    const jpt::JsonMap& subSet = jsonRoot["data_map"].As<jpt::JsonMap>();
    JPT_ENSURE(subSet["year"] == 2024);
    JPT_ENSURE(subSet["month"] == 6);
    JPT_ENSURE(subSet["day"] == 20);

    const jpt::JsonMap& subSet2 = subSet["Subset"].As<jpt::JsonMap>();
    JPT_ENSURE(subSet2["key1"] == 1);
    JPT_ENSURE(subSet2["key2"] == 2);
    JPT_ENSURE(subSet2["key3"] == 3);

    return true;
}

bool UnitTest_Json_Update()
{
    // Reads a json file. All the data should have been read into memory with this call
    jpt::JsonMap jsonRoot = jpt::ReadJsonFile(path).Value();

    // Update the data
    jsonRoot.Set("source", jpt::String("Jupiter Client"));

    // subset
    jpt::JsonMap& subSet = jsonRoot["data_map"].As<jpt::JsonMap>();
    subSet.Set("year", 1999);
    subSet.Set("Brand", jpt::String("MSI"));

    // Add new data
    jsonRoot.Set("new_data", jpt::String("New Data"));

    jpt::WriteJsonFile(path, jsonRoot);

    return true;
}

bool UnitTest_Json_ReadUpdated()
{
    // Reads a json file. All the data should have been read into memory with this call
    jpt::JsonMap jsonRoot = jpt::ReadJsonFile(path).Value();

    JPT_ENSURE(jsonRoot["source"] == jpt::String("Jupiter Client"));

    const jpt::JsonMap& subSet = jsonRoot["data_map"].As<jpt::JsonMap>();
    JPT_ENSURE(subSet["year"] == 1999);
    JPT_ENSURE(subSet["Brand"] == jpt::String("MSI"));
    JPT_ENSURE(subSet.Count() == 5);

    JPT_ENSURE(jsonRoot["new_data"] == jpt::String("New Data"));

    return true;
}

static bool Engine_Write()
{
    jpt::JsonMap engineJson;

    engineJson.Set("engine_version", 0);
    engineJson.Set("graphics_API", jpt::String("Vulkan"));
    jpt::WriteJsonFile(engineJsonPath, engineJson);

    return true;
}

static bool Engine_Update()
{
    jpt::JsonMap engineJson = jpt::ReadJsonFile(engineJsonPath).Value();

    engineJson.Set("engine_version", 1);
    engineJson.Set("graphics_API", jpt::String("OpenGL"));

    jpt::WriteJsonFile(engineJsonPath, engineJson);

    return true;
}

export bool RunUnitTests_Json()
{
    JPT_ENSURE(UnitTest_Json_Write());
    JPT_ENSURE(UnitTest_Json_Read());
    JPT_ENSURE(UnitTest_Json_Update());
    JPT_ENSURE(UnitTest_Json_ReadUpdated());

    JPT_ENSURE(Engine_Write());
    JPT_ENSURE(Engine_Update());

    return true;
}
