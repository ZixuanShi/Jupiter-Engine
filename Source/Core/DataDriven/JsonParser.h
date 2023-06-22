// Copyright Jupiter Technologies, Inc. All Rights Reserved.

#pragma once

namespace jpt
{
	class JPT_API JsonFile : public File
	{

	};

	JPT_API JsonFile LoadJsonFile(const char* pPath);
}

/*
- Struct for json file data. optional child inside
- Load a json file. need file path to it. Store everything to a hash map
- Get value by name
- Set value and store it to disk
*/