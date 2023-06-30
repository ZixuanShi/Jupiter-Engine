// Copyright Jupiter Technologies, Inc. All Rights Reserved.

#pragma once

namespace jpt
{
	class JPT_API JsonFile : public File
	{
	public:
		struct Data
		{

		};

	public:
		/**  */
		static jpt::optional<JsonFile> Load(const char* pPath);
	};
}

/*
- Struct for json file data. optional child inside
- Load a json file. need file path to it. Store everything to a hash map per layer/depth
- Get value by name at a certain depth
- Set value and store it to disk
*/