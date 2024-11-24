// Copyright Jupiter Technologies, Inc. All Rights Reserved.

module;

#include "Debugging/Assert.h"
#include "Debugging/Logger.h"

#include <fstream>
#include <string>

export module jpt.Json;

import jpt.Constants;
import jpt.DynamicArray;
import jpt.String;
import jpt.String.Helpers;
import jpt.ToString;
import jpt.Optional;
import jpt.Pair;
import jpt.Stack;
import jpt.Utilities;

import jpt.Json.Data;

import jpt.File.Path;
import jpt.File.IO;

namespace jpt
{
	using namespace File;

	export constexpr String ParseKeyStr(const String& line)
	{
		const size_t keyStart = line.Find("\"") + 1;
		const size_t keyEnd = line.Find("\"", keyStart);
		return line.SubStr(keyStart, keyEnd - keyStart);
	}

	export constexpr String ParseValueStr(const String& line)
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

	export constexpr JsonData ParseValueData(const String& valueStr)
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

		// Map
		else if (valueStr.Front() == '{' && valueStr.Back() == '}')
		{
			JsonMap map;
			String copy = valueStr.SubStr(1, valueStr.Count() - 2);
			copy.Replace(" ","");

			while (true)
			{
				const size_t keyStart = copy.Find("\"") + 1;
				const size_t keyEnd = copy.Find("\"", keyStart);
				const String keyStr = copy.SubStr(keyStart, keyEnd - keyStart);

				const size_t valueStart = copy.Find(":", keyEnd) + 1;
				size_t valueEnd = copy.Find(",", valueStart);
				if (valueEnd == npos)
				{
					valueEnd = copy.Count();
				}

				const String value = copy.SubStr(valueStart, valueEnd - valueStart);
				map.Add(keyStr, ParseValueData(value));

				if (valueEnd == copy.Count())
				{
					break;
				}
				copy = copy.SubStr(valueEnd + 1);
			}

			return map;
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
	export Optional<JsonMap> ReadJsonFile(const Path& path)
	{
		std::ifstream file(path.ConstBuffer(), std::ios::in);
		if (!file.is_open()) [[unlikely]]
		{
			JPT_LOG("Couldn't find Json File: %ls", path.ConstBuffer());
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
					jsonMaps.Peek().Add(keyStr, valueData);
				}
				// Start of map next line
				else
				{
					mapNames.Push(Move(keyStr));
				}
			}
			// Start of map
			else if (line.Has("{"))
			{
				jsonMaps.Emplace();
			}
			// End of map
			else if (line.Has("}"))
			{
				// If this is the end of root map
				if (jsonMaps.Count() == 1)
				{
					file.close();
					return jsonMaps.Peek();
				}

				JsonMap map = jsonMaps.Peek();
				jsonMaps.Pop();

				String mapName = mapNames.Peek();
				jsonMaps.Peek().Add(mapName, map);
				mapNames.Pop();
			}
		}

		// Empty file
		file.close();
		return {};
	}

	export void WriteJsonFile(const Path& path, const JsonMap& jsonRoot)
	{
		WriteTextFile(path, ToString(jsonRoot));
	}
}