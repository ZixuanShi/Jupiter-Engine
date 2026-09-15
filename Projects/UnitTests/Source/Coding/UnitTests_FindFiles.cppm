// Copyright Jupiter Technologies, Inc. All Rights Reserved.

export module UnitTests.FindFiles;

import UnitTests.TestFramework;
import jpt.TypeDefs;
import jpt.Logger;
import jpt.PlatformPaths;
import std;

std::vector<std::string> FindFiles(std::string_view , std::string_view )
{
    return {};
}

export void RunUnitTests_FindFiles(jpt::TestCase& test)
{
    test.Expect(FindFiles("some/directory", "keyword").empty(), "Expected no files to be found");
}
