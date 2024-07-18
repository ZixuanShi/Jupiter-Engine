// Copyright Jupiter Technologies, Inc. All Rights Reserved.

module;

#include "Core/Minimal/CoreHeaders.h"

#include <string>

export module jpt.LaunchArgs;

import jpt.TypeDefs;
import jpt.String;
import jpt.HashMap;

namespace jpt
{
	/** Launch Arguments parser and access */
	export class LaunchArgs
	{
	private:
		HashMap<String, String> m_arguments; /**< Key-Value pairs of arguments. Value could be empty if key is a flag */

	public:
		static LaunchArgs& GetInstance();

		/** Parse command line arguments and store them in a map
			@param argsCount	How many arguments got passed in when launching
			@param arguments	An array of arguments */
		void Parse(int32 argsCount, char* arguments[]);

		/** @param argumentStr	A string of arguments */
		void Parse(char* argumentStr);

		/**	@return		True if a key exists. Either has value or flag */
		bool Has(const String& key) const;

		/** @return		Value of the key */
		const String& Get(const String& key) const;
	};

	LaunchArgs& LaunchArgs::GetInstance()
	{
		static LaunchArgs s_instance;
		return s_instance;
	}

	void LaunchArgs::Parse(int32 argsCount, char* arguments[])
	{
		m_arguments.Reserve(argsCount);

		for (int32 i = 0; i < argsCount; ++i)
		{
			char* argument = arguments[i];
			String argumentStr = argument;
			argumentStr.TrimRight();

			String key;
			String value;

			const size_t equalPos = argumentStr.Find('=');

			// Flag
			if (equalPos == npos)
			{
				key = argumentStr;
			}
			// Key-Value pair
			else
			{
				key = argumentStr.SubStr(0, equalPos);
				value = argumentStr.SubStr(equalPos + 1);
			}

			JPT_ASSERT(!key.IsEmpty());
			JPT_ASSERT(!m_arguments.Has(key), "Duplicated launch argument found \"%s\"", key.ConstBuffer());
			m_arguments.Add(key, value);
		}
	}

	void LaunchArgs::Parse(char* argumentStr)
	{
		int32 argsCount = 0;
		char* arguments[256];

		const char* delimiter = "-";
		char* nextToken = nullptr;

		char* token = strtok_s(argumentStr, delimiter, &nextToken);

		while (token)
		{
			arguments[argsCount] = token;
			token = strtok_s(nullptr, delimiter, &nextToken);
			++argsCount;
		}

		Parse(argsCount, arguments);
	}

	bool LaunchArgs::Has(const String& key) const
	{
		return m_arguments.Has(key);
	}

	const String& LaunchArgs::Get(const String& key) const
	{
		JPT_ASSERT(m_arguments.Has(key), "Launch Argument doesn't exist \"%s\"", key.ConstBuffer());
		return m_arguments[key];
	}
}