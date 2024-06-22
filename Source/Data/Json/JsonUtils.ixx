// Copyright Jupiter Technologies, Inc. All Rights Reserved.

module;

#include "Debugging/Assert.h"
#include "Debugging/Logger.h"	// temp

#include <fstream>
#include <string>

export module jpt.JsonUtils;

import jpt.Constants;
import jpt.JsonObject;
import jpt.DynamicArray;
import jpt.String;
import jpt.StringUtils;
import jpt.Optional;
import jpt.Pair;
import jpt.Stack;

import jpt.File.Path;
import jpt.FileIO;

namespace jpt
{
	using namespace File;

	String ParseKeyStr(const String& line)
	{
		const size_t keyStart = line.Find("\"") + 1;
		const size_t keyEnd = line.Find("\"", keyStart);
		return line.SubStr(keyStart, keyEnd - keyStart);
	}

	String ParseValueStr(const String& line)
	{
		const size_t colonIndex = line.Find(":");
		const size_t valueStart = [&line, colonIndex]()
			{
				// Find the first non-space character
				size_t i = colonIndex + 1;

				while (IsEmpty(line[i]))
				{
					++i;
				}

				return i;
			}();

		size_t valueEnd = line.Find(",", valueStart);
		if (valueEnd == npos)
		{
			valueEnd = line.Count();
		}
		return line.SubStr(valueStart, valueEnd - valueStart);
	}

	JsonData ParseValueData(const String& valueStr)
	{
		// Boolean
		if (valueStr == "true")
		{
			return true;
		}
		else if (valueStr == "false")
		{
			return false;
		}

		// String
		else if (valueStr.Front() == '\"' && valueStr.Back() == '\"')
		{
			return valueStr.SubStr(1, valueStr.Count() - 2);
		}

		// Null
		else if (valueStr == "null")
		{
			return String("Invalid");
		}

		// Number
		else
		{
			// Integer
			if (valueStr.Find('.') == npos)
			{
				return valueStr.ToInt<int32>();
			}
			// Float
			else
			{
				return valueStr.ToFloat<float32>();
			}
		}
	}

	/** Reads a json file from disk. Initialize all the data to memory and assign to root json object then return it */
	export Optional<JsonObject> ReadJsonRoot(const Path& path)
	{
		std::ifstream file(path.ConstBuffer(), std::ios::in);
		if (!file.is_open())
		{
			JPT_ERROR("Failed to open json file: %ls", path.ConstBuffer());
			return {};
		}

		JsonObject jsonRoot;
		std::string stdLine;
		while (std::getline(file, stdLine))
		{
			String line = stdLine.c_str();

			if (const size_t colonIndex = line.Find(":"); colonIndex != npos)
			{
				const String keyStr = ParseKeyStr(line);
				const String valueStr = ParseValueStr(line);
				JsonData valueData = ParseValueData(valueStr);
				jsonRoot.Add(keyStr, valueData);
			}
		}

		return jsonRoot;
	}
}