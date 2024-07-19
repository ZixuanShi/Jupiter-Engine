// Copyright Jupiter Technologies, Inc. All Rights Reserved.

module;

#include "Core/Minimal/CoreHeaders.h"

export module UnitTests_LaunchArgs;

import jpt.LaunchArgs;
import jpt.TypeDefs;
import jpt.Utilities;
import jpt.String;
import jpt.DynamicArray;

bool UnitTests_WinMain_0()
{
    const char* args = "-bool_flag_0 -graphics_api=dx12 -magic_value=4.2 -magic_str=wow -bool_flag_1";

    jpt::LaunchArgs localLaunchArgs;
    localLaunchArgs.Parse(args);

    JPT_ENSURE(localLaunchArgs.Has("bool_flag_0"));
    JPT_ENSURE(localLaunchArgs.Has("graphics_api"));
    JPT_ENSURE(localLaunchArgs.Has("magic_value"));
    JPT_ENSURE(localLaunchArgs.Has("magic_str"));
    JPT_ENSURE(localLaunchArgs.Has("bool_flag_1"));

    JPT_ENSURE(localLaunchArgs.Get("bool_flag_0") == "");
    JPT_ENSURE(localLaunchArgs.Get("graphics_api") == "dx12");
    JPT_ENSURE(localLaunchArgs.Get("magic_value") == "4.2");
    JPT_ENSURE(localLaunchArgs.Get("magic_str") == "wow");
    JPT_ENSURE(localLaunchArgs.Get("bool_flag_1") == "");

    return true;
}

bool UnitTests_WinMain_1()
{
    const char* args = "-bool_flag_0" 
        "-graphics_api=dx12"
        "-magic_value=4.2"
        "-magic_str=w ow          -bool_flag_1";

    jpt::LaunchArgs localLaunchArgs;
    localLaunchArgs.Parse(args);

    JPT_ENSURE(localLaunchArgs.Has("bool_flag_0"));
    JPT_ENSURE(localLaunchArgs.Has("graphics_api"));
    JPT_ENSURE(localLaunchArgs.Has("magic_value"));
    JPT_ENSURE(localLaunchArgs.Has("magic_str"));
    JPT_ENSURE(localLaunchArgs.Has("bool_flag_1"));

    JPT_ENSURE(localLaunchArgs.Get("bool_flag_0") == "");
    JPT_ENSURE(localLaunchArgs.Get("graphics_api") == "dx12");
    JPT_ENSURE(localLaunchArgs.Get("magic_value") == "4.2");
    JPT_ENSURE(localLaunchArgs.Get("magic_str") == "w ow");
    JPT_ENSURE(localLaunchArgs.Get("bool_flag_1") == "");

    return true;
}

bool UnitTests_Main_0()
{
    jpt::LaunchArgs localLaunchArgs;

    static constexpr int32 argc = 6;
    char* args[argc] = 
    {
        new char[] { "D:\\Programming\\Practice\\C++\\CppInternal\\Builds\\C++Test_Debug_x64_Output\\C++Test.exe" },
        new char[] { "-bool_flag_0" },
        new char[] { "-graphics_api=dx12" },
        new char[] { "-magic_value=4.2" },
        new char[] { "-magic_str=wow" },
        new char[] { "-bool_flag_1" },
    };

    localLaunchArgs.Parse(argc, args);

    JPT_ENSURE(localLaunchArgs.Has("bool_flag_0"));
    JPT_ENSURE(localLaunchArgs.Has("graphics_api"));
    JPT_ENSURE(localLaunchArgs.Has("magic_value"));
    JPT_ENSURE(localLaunchArgs.Has("magic_str"));
    JPT_ENSURE(localLaunchArgs.Has("bool_flag_1"));

    JPT_ENSURE(localLaunchArgs.Get("bool_flag_0") == "");
    JPT_ENSURE(localLaunchArgs.Get("graphics_api") == "dx12");
    JPT_ENSURE(localLaunchArgs.Get("magic_value") == "4.2");
    JPT_ENSURE(localLaunchArgs.Get("magic_str") == "wow");
    JPT_ENSURE(localLaunchArgs.Get("bool_flag_1") == "");

    for (int32 i = 0; i < argc; ++i)
	{
		delete[] args[i];
	}

    return true;
}

export bool RunUnitTests_LaunchArgs()
{
    JPT_ENSURE(UnitTests_WinMain_0());
    JPT_ENSURE(UnitTests_WinMain_1());

    JPT_ENSURE(UnitTests_Main_0());

    return true;
}
