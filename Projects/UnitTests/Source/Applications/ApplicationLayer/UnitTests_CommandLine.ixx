// Copyright Jupiter Technologies, Inc. All Rights Reserved.

module;

#include "Core/Minimal/CoreHeaders.h"

export module UnitTests_CommandLine;

import jpt.CommandLine;
import jpt.String;
import jpt.DynamicArray;
import jpt.HashMap;

import jpt.Json;
import jpt.JsonData;

// Paste this to command line to run this unit test
// -flag -keyStr="string" -keyStr2="Hello World" -keyFloat=50.55 -keyBool=false -keyInt=42 -keyArray=[1, 2, 3, 4, 5] -keyMap={ "key1": 42, "key2": 2.5, "key3": true, "key4": "mapValue" }

bool CommandLine()
{
    static const jpt::String commandLine = "-flag -keyStr=\"string\" -keyStr2=\"Hello World\" -keyFloat=50.55 -keyBool=false -keyInt=42 -keyArray=[1, 2, 3, 4, 5] -keyMap={ \"key1\": 42, \"key2\": 2.5, \"key3\": true, \"key4\": \"mapValue\" }";

    jpt::CommandLine& cmdLine = jpt::CommandLine::GetInstance();
    cmdLine.Parse(commandLine.ConstBuffer());

    JPT_ENSURE(cmdLine.Has("flag"));

    JPT_ENSURE(cmdLine.Has("keyStr"));
    JPT_ENSURE(cmdLine.Get<jpt::String>("keyStr") == "string");

    JPT_ENSURE(cmdLine.Has("keyStr2"));
    JPT_ENSURE(cmdLine.Get<jpt::String>("keyStr2") == "Hello World");

    JPT_ENSURE(cmdLine.Has("keyFloat"));
    JPT_ENSURE(cmdLine.Get<float>("keyFloat") == 50.55f);

    JPT_ENSURE(cmdLine.Has("keyBool"));
    JPT_ENSURE(cmdLine.Get<bool>("keyBool") == false);

    JPT_ENSURE(cmdLine.Has("keyInt"));
    JPT_ENSURE(cmdLine.Get<int>("keyInt") == 42);

    JPT_ENSURE(cmdLine.Has("keyArray"));
    const jpt::JsonArray& keyArray = cmdLine.Get<jpt::JsonArray>("keyArray");
    JPT_ENSURE(keyArray.Count() == 5);
    JPT_ENSURE(keyArray[0] == 1);
    JPT_ENSURE(keyArray[1] == 2);
    JPT_ENSURE(keyArray[2] == 3);
    JPT_ENSURE(keyArray[3] == 4);
    JPT_ENSURE(keyArray[4] == 5);

    JPT_ENSURE(cmdLine.Has("keyMap"));
    const jpt::JsonMap& keyMap = cmdLine.Get<jpt::JsonMap>("keyMap");
    JPT_ENSURE(keyMap.Count() == 4);
    JPT_ENSURE(keyMap["key1"] == 42);
    JPT_ENSURE(keyMap["key2"] == 2.5f);
    JPT_ENSURE(keyMap["key3"] == true);
    JPT_ENSURE(keyMap["key4"] == jpt::String("mapValue"));

    cmdLine.Erase("flag");
    cmdLine.Erase("keyStr");
    cmdLine.Erase("keyStr2");
    cmdLine.Erase("keyFloat");
    cmdLine.Erase("keyBool");
    cmdLine.Erase("keyInt");
    cmdLine.Erase("keyArray");
    cmdLine.Erase("keyMap");

    JPT_ENSURE(!cmdLine.Has("flag"));
    JPT_ENSURE(!cmdLine.Has("keyStr"));
    JPT_ENSURE(!cmdLine.Has("keyStr2"));
    JPT_ENSURE(!cmdLine.Has("keyFloat"));
    JPT_ENSURE(!cmdLine.Has("keyBool"));
    JPT_ENSURE(!cmdLine.Has("keyInt"));
    JPT_ENSURE(!cmdLine.Has("keyArray"));
    JPT_ENSURE(!cmdLine.Has("keyMap"));

    return true;
}

export bool RunUnitTests_CommandLine()
{
    JPT_ENSURE(CommandLine());

    return true;
}
