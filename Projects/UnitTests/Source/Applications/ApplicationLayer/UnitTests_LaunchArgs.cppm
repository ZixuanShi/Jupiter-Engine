// Copyright Jupiter Technologies, Inc. All Rights Reserved.

module;

#include "Core/Minimal/CoreHeaders.h"

export module UnitTests_LaunchArgs;

import jpt.LaunchArgs;
import jpt.String;
import jpt.DynamicArray;
import jpt.HashMap;

import jpt.Json;
import jpt.JsonData;

// Paste this to command line to run this unit test
// -flag -keyStr="string" -keyStr2="Hello World" -keyFloat=50.55 -keyBool=false -keyInt=42 -keyArray=[1, 2, 3, 4, 5] -keyMap={ "key1": 42, "key2": 2.5, "key3": true, "key4": "mapValue" }

bool LaunchArgs()
{
    static const jpt::String commandLine = "-flag -keyStr=\"string\" -keyStr2=\"Hello World\" -keyFloat=50.55 -keyBool=false -keyInt=42 -keyArray=[1, 2, 3, 4, 5] -keyMap={ \"key1\": 42, \"key2\": 2.5, \"key3\": true, \"key4\": \"mapValue\" }";

    jpt::LaunchArgs& launchArgs = jpt::LaunchArgs::GetInstance();
    launchArgs.Parse(commandLine.ConstBuffer());

    JPT_ENSURE(launchArgs.Has("flag"));

    JPT_ENSURE(launchArgs.Has("keyStr"));
    JPT_ENSURE(launchArgs.Get<jpt::String>("keyStr") == "string");

    JPT_ENSURE(launchArgs.Has("keyStr2"));
    JPT_ENSURE(launchArgs.Get<jpt::String>("keyStr2") == "Hello World");

    JPT_ENSURE(launchArgs.Has("keyFloat"));
    JPT_ENSURE(launchArgs.Get<float>("keyFloat") == 50.55f);

    JPT_ENSURE(launchArgs.Has("keyBool"));
    JPT_ENSURE(launchArgs.Get<bool>("keyBool") == false);

    JPT_ENSURE(launchArgs.Has("keyInt"));
    JPT_ENSURE(launchArgs.Get<int>("keyInt") == 42);

    JPT_ENSURE(launchArgs.Has("keyArray"));
    const jpt::JsonArray& keyArray = launchArgs.Get<jpt::JsonArray>("keyArray");
    JPT_ENSURE(keyArray.Count() == 5);
    JPT_ENSURE(keyArray[0] == 1);
    JPT_ENSURE(keyArray[1] == 2);
    JPT_ENSURE(keyArray[2] == 3);
    JPT_ENSURE(keyArray[3] == 4);
    JPT_ENSURE(keyArray[4] == 5);

    JPT_ENSURE(launchArgs.Has("keyMap"));
    const jpt::JsonMap& keyMap = launchArgs.Get<jpt::JsonMap>("keyMap");
    JPT_ENSURE(keyMap.Count() == 4);
    JPT_ENSURE(keyMap["key1"] == 42);
    JPT_ENSURE(keyMap["key2"] == 2.5f);
    JPT_ENSURE(keyMap["key3"] == true);
    JPT_ENSURE(keyMap["key4"] == jpt::String("mapValue"));

    launchArgs.Erase("flag");
    launchArgs.Erase("keyStr");
    launchArgs.Erase("keyStr2");
    launchArgs.Erase("keyFloat");
    launchArgs.Erase("keyBool");
    launchArgs.Erase("keyInt");
    launchArgs.Erase("keyArray");
    launchArgs.Erase("keyMap");

    JPT_ENSURE(!launchArgs.Has("flag"));
    JPT_ENSURE(!launchArgs.Has("keyStr"));
    JPT_ENSURE(!launchArgs.Has("keyStr2"));
    JPT_ENSURE(!launchArgs.Has("keyFloat"));
    JPT_ENSURE(!launchArgs.Has("keyBool"));
    JPT_ENSURE(!launchArgs.Has("keyInt"));
    JPT_ENSURE(!launchArgs.Has("keyArray"));
    JPT_ENSURE(!launchArgs.Has("keyMap"));

    return true;
}

export bool RunUnitTests_LaunchArgs()
{
    JPT_ENSURE(LaunchArgs());

    return true;
}
