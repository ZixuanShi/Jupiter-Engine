// Copyright Jupiter Technologies, Inc. All Rights Reserved.

module;

#include "Core/Minimal/CoreMacros.h"
#include "Core/Validation/Assert.h"
#include "Debugging/Logger.h"

export module jpt.CommandLine;

import jpt.DynamicArray;
import jpt.HashMap;
import jpt.String;
import jpt.String.Helpers;
import jpt.ToString;
import jpt.TypeDefs;
import jpt.Utilities;

import jpt.Json;
import jpt.Json.Data;

export namespace jpt
{
    /** Launch Arguments parser and access */
    class CommandLine
    {
    private:
        JsonMap m_arguments; /**< Key-Value pairs of arguments. Value could be empty if key is a flag */

    public:
        JPT_DECLARE_SINGLETON(CommandLine);

        /** Parse command line arguments and store them in a map
            Expected Launch Args format: { "-key=value", "-flag", "-key_2=value", "-flag2" }
            @param argsCount    How many arguments got passed in when launching
            @param arguments    An array of arguments */
        void Parse(int32 argsCount, char* arguments[]);

        /** @param argumentStr    A string of arguments
            Expected Launch Args format: "-key=value -flag -key_2=value -flag2" */
        void Parse(const char* argumentStr);

        /** Set a key-value pair to the arguments map. Could be empty if key is a flag. Will update value if key already exists */
        void Set(const String& key, const JsonData& value = JsonData());

        /** @return Value associated with the key */
        template<ValidJsonType T> const T& Get(const String& key) const;
        template<ValidJsonType T> const T& Get(const String& key, const T& defaultValue) const;

        /**    @return        True if a key exists. Either has value or flag */
        bool Has(const String& key) const;

        /** Removes a key from the arguments map */
        void Erase(const String& key);

        /** @return        All arguments in a map format */
        const JsonMap& GetArgs() const { return m_arguments; }

    private:
        /** Parse a single argument "-key=value" and store into arguments map */
        void Parse(String&& argument);
    };

    void CommandLine::Parse(int32 argsCount, char* arguments[])
    {
        m_arguments.ResizeBuckets(argsCount - 1);

        // Skip the first argument which is the executable path. It will be added by Engine/System/Environment/SystemPaths.ixx

        for (int32 i = 1; i < argsCount; ++i)
        {
            String argumentStr = arguments[i];
            Parse(Move(argumentStr));
        }
    }

    void CommandLine::Parse(const char* argumentStr)
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

    void CommandLine::Set(const String& key, const JsonData& value /* = JsonData()*/)
    {
        m_arguments[key] = value;
    }

    bool CommandLine::Has(const String& key) const
    {
        return m_arguments.Has(key);
    }

    void CommandLine::Erase(const String& key)
    {
        JPT_ASSERT(m_arguments.Has(key), "CommandLine doesn't exist \"%s\"", key.ConstBuffer());
        m_arguments.Erase(key);
    }

    void CommandLine::Parse(String&& argument)
    {
        // Remove the leading '-'
        if (argument.Front() == '-')
        {
            argument.TrimLeft(1);
        }
        argument.TrimRight();

        String key;
        JsonData value;

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
            const String valueStr = argument.SubStr(equalPos + 1);
            value = ParseValueData(valueStr);
        }

        Set(key, value);
    }

    template<ValidJsonType T>
    const T& CommandLine::Get(const String& key) const
    {
        JPT_ASSERT(Has(key), "CommandLine doesn't exist \"%s\"", key.ConstBuffer());
        JPT_ASSERT(m_arguments[key].Is<T>(), "CommandLine \"%s\" is not of type %s", key.ConstBuffer());
        return m_arguments[key].As<T>();
    }

    template<ValidJsonType T>
    const T& CommandLine::Get(const String& key, const T& defaultValue) const
    {
        if (Has(key))
        {
            return Get<T>(key);
        }

        return defaultValue;
    }
}