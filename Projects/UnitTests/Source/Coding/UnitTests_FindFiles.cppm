// Copyright Jupiter Technologies, Inc. All Rights Reserved.

export module UnitTests.FindFiles;

import UnitTests.TestFramework;
import jpt.TypeDefs;
import jpt.Logger;
import jpt.PlatformPaths;
import std;

std::vector<std::string> FindFiles(std::string_view directory, std::string_view keyword)
{
    return {};
}

export void RunUnitTests_FindFiles(jpt::TestCase& )
{
    jpt::Debug::Log("jpt::GetRoot(): {}", jpt::GetRoot().string());
    jpt::Debug::Log("jpt::GetSavedDir(): {}", jpt::GetSavedDir().string());
    jpt::Debug::Log("jpt::GetEngineDir(): {}", jpt::GetEngineDir().string());
    jpt::Debug::Log("jpt::GetProjectDir(): {}", jpt::GetProjectDir().string());
}
