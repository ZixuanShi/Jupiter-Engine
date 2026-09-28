// Copyright Jupiter Technologies, Inc. All Rights Reserved.

export module UnitTests.FindFiles;

import UnitTests.TestFramework;
import jpt.TypeDefs;
import jpt.Logger;
import jpt.PlatformPaths;
import std;

/* It should return the files within a folder that match different filters, like size min/max cap, date of last modification, key word in filename, file formats, etc */
std::vector<std::string> FindFiles(std::string_view , std::string_view )
{
    return {};
}

export void RunUnitTests_FindFiles(jpt::TestCase& test)
{
    test.Expect(FindFiles("some/directory", "keyword").empty(), "Expected no files to be found");
}
