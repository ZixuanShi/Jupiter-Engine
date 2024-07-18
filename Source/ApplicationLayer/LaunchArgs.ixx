// Copyright Jupiter Technologies, Inc. All Rights Reserved.

module;

#include "Core/Minimal/CoreHeaders.h"

#include <string>

export module jpt.LaunchArgs;

import jpt.String;
import jpt.HashMap;

namespace jpt
{
	/** Parses command line arguments */
	export class LaunchArgs
	{
	private:
		HashMap<String, String> m_arguments;
		size_t m_count = 0;

	public:
		static LaunchArgs& GetInstance();

		/** Parses command line arguments when launching */
		void Parse(int argsCount, char* arguments[]);
		void Parse(char* argumentStr);
	};

	LaunchArgs& LaunchArgs::GetInstance()
	{
		static LaunchArgs s_instance;
		return s_instance;
	}

	void LaunchArgs::Parse(int argsCount, char* arguments[])
	{
		JPT_IGNORE(argsCount, arguments);
	}

	void LaunchArgs::Parse(char* argumentStr)
	{
		int argsCount = 0;
		char* arguments[256];

		const char* delimiter = " ";
		char* nextToken = nullptr;

		char* token = strtok_s(argumentStr, delimiter, &nextToken);

		while (token)
		{
			arguments[argsCount++] = token;
			token = strtok_s(nullptr, delimiter, &nextToken);
		}

		Parse(argsCount, arguments);
	}
}