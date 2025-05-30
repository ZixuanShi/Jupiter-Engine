// Copyright Jupiter Technologies, Inc. All Rights Reserved.

module;

#include "Core/Minimal/CoreHeaders.h"

export module UnitTests_Json;

import jpt.TypeDefs;
import jpt.Utilities;
import jpt.Math;

import jpt.Json;
import jpt.Json.Data;

import jpt.File.Path;
import jpt.File.Path.Utils;
import jpt.File.IO;
import jpt.File.Enums;

import jpt.Graphics.Enums;

static const jpt::File::Path path = jpt::File::Combine(jpt::File::Source::Output, "Assets/TestJson_UnitTest.json");
static const jpt::File::Path engineJsonPath = jpt::File::Combine(jpt::File::Source::Output, "Assets/TestJson_UnitTest.json");

bool UnitTests_Json_Write()
{
    jpt::JsonMap jsonRoot;

    jsonRoot.Add("source", jpt::String("Client"));
    jsonRoot.Add("data_int", 12);
    jsonRoot.Add("data_float", 1.55f);
    jsonRoot.Add("data_string", jpt::String("Hello from Jupiter Engine!"));
    jsonRoot.Add("data_null", jpt::String("null"));

    jsonRoot.Add("data_array", jpt::JsonArray{111, jpt::String("Hello World"), 33, 4.77f, false});

    jpt::JsonMap subMap2;
    subMap2.Add("key1", 1);
    subMap2.Add("key2", 2);
    subMap2.Add("key3", 3);

    jpt::JsonMap subMap;
    subMap.Add("year", 2024);
    subMap.Add("month", 6);
    subMap.Add("day", 20);
    subMap.Add("Subset", subMap2);

    jsonRoot.Add("data_map", subMap);

    jpt::WriteJsonFile(path, jsonRoot);

    return true;
}

bool UnitTests_Json_Read()
{
    // Reads a json file. All the data should have been read into memory with this call
    jpt::JsonMap jsonRoot = jpt::ReadJsonFile(path).Value();

    // Access the data with operator[]
    JPT_ENSURE(jsonRoot["source"] == jpt::String("Client"));
    JPT_ENSURE(jsonRoot["data_int"] == 12);
    JPT_ENSURE(jpt::AreValuesClose(jsonRoot["data_float"].As<float32>(), 1.55f));
    JPT_ENSURE(jsonRoot["data_string"] == jpt::String("Hello from Jupiter Engine!"));
    JPT_ENSURE(jsonRoot["data_null"] == jpt::String("null"));

    const jpt::JsonArray& data_array = jsonRoot["data_array"];
    jpt::JsonArray expected_data_array{ 111, jpt::String("Hello World"), 33, 4.77f, false};
    JPT_ENSURE(data_array == expected_data_array);

    // Subset map
    const jpt::JsonMap& subSet = jsonRoot["data_map"];
    JPT_ENSURE(subSet["year"] == 2024);
    JPT_ENSURE(subSet["month"] == 6);
    JPT_ENSURE(subSet["day"] == 20);

    const jpt::JsonMap& subSet2 = subSet["Subset"];
    JPT_ENSURE(subSet2["key1"] == 1);
    JPT_ENSURE(subSet2["key2"] == 2);
    JPT_ENSURE(subSet2["key3"] == 3);

    return true;
}

bool UnitTests_Json_Update()
{
    // Reads a json file. All the data should have been read into memory with this call
    jpt::JsonMap jsonRoot = jpt::ReadJsonFile(path).Value();

    // Update the data
    jsonRoot["source"] = jpt::String("Jupiter Client");

    // subset
    jpt::JsonMap& subSet = jsonRoot["data_map"];
    subSet["year"] = 1999;
    subSet["Brand"] = jpt::String("MSI");

    // Add new data
    jsonRoot["new_data"] = jpt::String("New Data");

    jpt::WriteJsonFile(path, jsonRoot);

    return true;
}

bool UnitTests_Json_ReadUpdated()
{
    // Reads a json file. All the data should have been read into memory with this call
    jpt::JsonMap jsonRoot = jpt::ReadJsonFile(path).Value();

    JPT_ENSURE(jsonRoot["source"] == jpt::String("Jupiter Client"));

    const jpt::JsonMap& subSet = jsonRoot["data_map"];
    JPT_ENSURE(subSet["year"] == 1999);
    JPT_ENSURE(subSet["Brand"] == jpt::String("MSI"));
    JPT_ENSURE(subSet.Count() == 5);

    JPT_ENSURE(jsonRoot["new_data"] == jpt::String("New Data"));

    return true;
}

static bool Engine_Write()
{
    jpt::JsonMap engineJson;

    engineJson.Add("engine_version", 0);

    jpt::GraphicsAPI graphicsAPI = jpt::GraphicsAPI::Vulkan;
    engineJson.Add("graphics_API", ToString(graphicsAPI));

    jpt::WriteJsonFile(engineJsonPath, engineJson);

    return true;
}

static bool Engine_Read()
{
    jpt::JsonMap engineJson = jpt::ReadJsonFile(engineJsonPath).Value();

    JPT_ENSURE(engineJson["engine_version"] == 0);

    const jpt::String& graphicsAPIStr = engineJson["graphics_API"];
    JPT_ENSURE(graphicsAPIStr == "Vulkan");

    jpt::GraphicsAPI graphicsAPI(graphicsAPIStr);
    JPT_ENSURE(graphicsAPI == jpt::GraphicsAPI::Vulkan);

    return true;
}

export bool RunUnitTests_Json()
{
    JPT_ENSURE(UnitTests_Json_Write());
    JPT_ENSURE(UnitTests_Json_Read());
    JPT_ENSURE(UnitTests_Json_Update());
    JPT_ENSURE(UnitTests_Json_ReadUpdated());

    JPT_ENSURE(Engine_Write());
    JPT_ENSURE(Engine_Read());

    jpt::File::Delete(path);
    jpt::File::Delete(engineJsonPath);

    return true;
}
