// Copyright Jupiter Technologies, Inc. All Rights Reserved.

#include "JupiterPCH.h"
#include "JsonParser.h"

#include <fstream>

namespace jpt
{
	jpt::optional<JsonFile> JsonFile::Load(const char* pPath)
	{
		// Relative path starts at VS project file location
		std::ofstream os("../test.txt", std::ios::out);
		os.write("ABCDEFG\n", 8);
		os.write("ABCDEFG\n", 8);
		os.write("ABCDEFG\n", 8);
		os.write("ABCDEFG\n", 5);	// Only writes 5 chars. Ignoring the rest
		os.close();

		return JsonFile(pPath);
	}
}
