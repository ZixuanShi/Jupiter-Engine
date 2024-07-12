// Copyright Jupiter Technologies, Inc. All Rights Reserved.

module;

#include "Debugging/Assert.h"
#include "Debugging/Logger.h"	// temp

#include <fstream>
#include <string>

export module jpt.JsonUtils;

import jpt.Constants;
import jpt.DynamicArray;
import jpt.String;
import jpt.StringUtils;
import jpt.ToString;
import jpt.Optional;
import jpt.Pair;
import jpt.Stack;
import jpt.Utilities;

import jpt.JsonData;

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

				while (line[i] == ' ')
				{
					++i;
				}

				return i;
			}();

		size_t valueEnd = npos;

		// Is empty. Could be start of map
		if (valueStart == line.Count())
		{
			return String();
		}
		// Is array
		else if (line[valueStart] == '[')
		{
			valueEnd = line.Find("]", valueStart) + 1;
		}
		// single data
		else
		{
			valueEnd = line.Find(",", valueStart);
			if (valueEnd == npos)	// Last element. No comma
			{
				valueEnd = line.Count();
			}
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
			return String("null");
		}

		// Array
		else if (valueStr.Front() == '[' && valueStr.Back() == ']')
		{
			DynamicArray<JsonData> arr;
			String copy = valueStr.SubStr(1, valueStr.Count() - 2);

			while (true)
			{
				const size_t valueStart = [&copy]()
					{
						// Find the first non-space character
						size_t i = 0;

						while (copy[i] == ' ')
						{
							++i;
						}

						return i;
					}();

				size_t valueEnd = copy.Find(",", valueStart);
				if (valueEnd == npos)
				{
					// Last element
					arr.Add(ParseValueData(copy.SubStr(valueStart, copy.Count() - valueStart)));
					break;
				}

				String value = copy.SubStr(valueStart, valueEnd - valueStart);
				arr.Add(ParseValueData(value));
				copy = copy.SubStr(valueEnd + 1);
			}

			return arr;
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
	export Optional<JsonMap> ReadJsonRoot(const Path& path)
	{
		std::ifstream file(path.ConstBuffer(), std::ios::in);
		if (!file.is_open())
		{
			JPT_ERROR("Failed to open json file: %ls", path.ConstBuffer());
			return {};
		}

		Stack<JsonMap> jsonMaps;
		Stack<String> mapNames;

		std::string stdLine;
		while (std::getline(file, stdLine))
		{
			const String line = stdLine.c_str();

			if (const size_t colonIndex = line.Find(":"); colonIndex != npos)
			{
				const String keyStr = ParseKeyStr(line);
				const String valueStr = ParseValueStr(line);

				if (!valueStr.IsEmpty())
				{
					JsonData valueData = ParseValueData(valueStr);
					jsonMaps.Peek().Set(keyStr, valueData);
				}
				// Start of map next line
				else
				{
					mapNames.Push(Move(keyStr));
				}
			}
			// Start of map
			else if (line.Contains("{"))
			{
				jsonMaps.Emplace();
			}
			// End of map
			else if (line.Contains("}"))
			{
				// If this is the end of root map
				if (jsonMaps.Count() == 1)
				{
					return jsonMaps.Peek();
				}

				JsonMap map = jsonMaps.Peek();
				jsonMaps.Pop();

				String mapName = mapNames.Peek();
				jsonMaps.Peek().Set(mapName, map);
				mapNames.Pop();
			}
		}

		// Empty file
		return {};
	}

	export String ToString(const JsonMap& map)
	{
		String str;
		str.Append("{\n");

		size_t count = 0;
		for (const auto& [key, value] : map)
		{
			str.Append("\t\"");
			str.Append(key);
			str.Append("\": ");
			str.Append(ToString(value));

			++count;
			if (count < map.Count())
			{
				str.Append(",\n");
			}
			else
			{
				str.Append("\n");
			}
		}

		str.Append("}");
		return str;
	}

	export void WriteJsonRoot(const Path& path, const JsonMap& jsonRoot)
	{
		std::ofstream file(path.ConstBuffer(), std::ios::out);
		if (!file.is_open())
		{
			JPT_ERROR("Failed to open json file: %ls", path.ConstBuffer());
			return;
		}

		file << ToString(jsonRoot).ConstBuffer();
	}
}