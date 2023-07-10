// Copyright Jupiter Technologies, Inc. All Rights Reserved.

#include "JupiterPCH.h"
#include "JsonParser.h"

#include <fstream>

namespace jpt
{
	jpt::optional<JsonFile> JsonFile::Load(EAssetDirectoryType directoryType, const char* pRelativePath)
	{
		std::ofstream outJson("TestJsonZixuan.json", std::ios::out);
		outJson.write("{\n", 2);
		outJson.write("\t\"Name\": \"Jupiter\",\n", 20);
		outJson.write("\t\"Author\": \"Zixuan\",\n", 21);
		outJson.write("\t\"Version\": 0,\n", 15);
		outJson.write("\t\"IsPromising\": true,\n", 22);

		outJson.write("\t\"Player1\":\n", 12);
		outJson.write("\t{\n", 3);
		outJson.write("\t\t\"Name\": \"Jax\",\n", 17);
		outJson.write("\t\t\"Location\": [12, 0, 1],\n", 26);
		outJson.write("\t\t\"Health\": 352,\n", 17);
		outJson.write("\t\t\"Inventory\":\n", 15);
		outJson.write("\t\t{\n", 4);
		outJson.write("\t\t\t\"Stick\": 1,\n", 15);
		outJson.write("\t\t\t\"HealthPortion\": 2,\n", 23);
		outJson.write("\t\t\t\"Pink\": 1\n", 13);
		outJson.write("\t\t}\n", 4);
		outJson.write("\t},\n", 4);

		outJson.write("\t\"Player2\":\n", 12);
		outJson.write("\t{\n", 3);
		outJson.write("\t\t\"Name\": \"Darius\",\n", 20);
		outJson.write("\t\t\"Location\": [-2, -55, 15],\n", 29);
		outJson.write("\t\t\"Health\": 270,\n", 17);
		outJson.write("\t\t\"Inventory\":\n", 15);
		outJson.write("\t\t{\n", 4);
		outJson.write("\t\t\t\"Axe\": 1,\n", 13);
		outJson.write("\t\t\t\"ManaPotion\": 5\n", 19);
		outJson.write("\t\t}\n", 4);
		outJson.write("\t},\n", 4);

		outJson.write("\t\"PI\": 3.1415\n", 14);
		outJson.write("}\n", 2);
		outJson.close();

		const jpt::string absolutePath = GetFullPathFromDirectory(directoryType, pRelativePath);
		return Load(absolutePath.c_str());
	}

	jpt::optional<JsonFile> JsonFile::Load(const char* pAbsolutePath)
	{
		std::ifstream inJson(pAbsolutePath, std::ios::in);
		JPT_ASSERT(inJson.is_open());
		
		std::string line;
		while (std::getline(inJson, line))
		{
			JPT_LOG(line.c_str());
		}

		inJson.close();

		return JsonFile(pAbsolutePath);
	}
}
