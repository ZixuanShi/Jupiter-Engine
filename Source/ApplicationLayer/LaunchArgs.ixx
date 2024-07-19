// Copyright Jupiter Technologies, Inc. All Rights Reserved.

module;

#include "Core/Minimal/CoreMacros.h"
#include "Debugging/Assert.h"
#include "Debugging/Logger.h"

export module jpt.LaunchArgs;

import jpt.TypeDefs;
import jpt.String;
import jpt.StringUtils;
import jpt.HashMap;
import jpt.DynamicArray;
import jpt.Utilities;

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
			Expected Launch Args format: { "-key=value", "-flag", "-key_2=value", "-flag2" }
			@param argsCount	How many arguments got passed in when launching
			@param arguments	An array of arguments */
		void Parse(int32 argsCount, char* arguments[]);

		/** @param argumentStr	A string of arguments
			Expected Launch Args format: "-key=value -flag -key_2=value -flag2" */
		void Parse(const char* argumentStr);

		/**	@return		True if a key exists. Either has value or flag */
		bool Has(const String& key) const;

		/** @return		Value of the key */
		const String& Get(const String& key) const;

	private:
		/** Parse a single argument "-key=value" and store into arguments map */
		void Parse(String&& argument);
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
			String argumentStr = arguments[i];
			Parse(Move(argumentStr));
		}
	}

	void LaunchArgs::Parse(const char* argumentStr)
	{
		if (FindCharsCount(argumentStr) == 0)
		{
			return;
		}

		String argumentsCopy = argumentStr;

		while (true)
		{
			const Index start = argumentsCopy.FindFirstOf('-');
			if (start == npos)
			{
				break;
			}

			Index end = argumentsCopy.FindFirstOf('-', start + 1);
			if (end == npos)
			{
				end = argumentsCopy.Count();
			}

			String argument = argumentsCopy.SubStr(start, end - start);
			Parse(Move(argument));

			argumentsCopy = argumentsCopy.SubStr(end);
		}
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

	void LaunchArgs::Parse(String&& argument)
	{
		// Remove the leading '-'
		if (argument.Front() == '-')
		{
			argument.TrimLeft(1);
		}
		argument.TrimRight();

		String key;
		String value;

		const size_t equalPos = argument.Find('=');

		// Flag
		if (equalPos == npos)
		{
			key = argument;
		}
		// Key-Value pair
		else
		{
			key = argument.SubStr(0, equalPos);
			value = argument.SubStr(equalPos + 1);
		}

		JPT_ASSERT(!key.IsEmpty());
		JPT_ASSERT(!m_arguments.Has(key), "Duplicated launch argument found \"%s\"", key.ConstBuffer());
		m_arguments.Add(key, value);
	}
}