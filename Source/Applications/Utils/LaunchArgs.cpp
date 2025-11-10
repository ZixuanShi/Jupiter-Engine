// Copyright Jupiter Technologies, Inc. All Rights Reserved.

module;

#include "Debugging/Logger.h"

module jpt.LaunchArgs;

import jpt.DynamicArray;
import jpt.HashMap;
import jpt.StringHelpers;
import jpt.ToString;

import jpt.Json;

namespace jpt
{
    void LaunchArgs::Parse(int32 argsCount, char* arguments[])
    {
        m_arguments.ResizeBuckets(argsCount - 1);

        // Skip the first argument which is the executable path. It will be added by Engine/System/Environment/SystemPaths.ixx

        for (int32 i = 1; i < argsCount; ++i)
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
            if (start == kInvalidIndex)
            {
                break;
            }

            Index end = argumentsCopy.FindFirstOf('-', start + 1);
            if (end == kInvalidIndex)
            {
                end = argumentsCopy.Count();
            }

            String argument = argumentsCopy.SubStr(start, end - start);
            Parse(Move(argument));

            argumentsCopy = argumentsCopy.SubStr(end);
        }
    }

    void LaunchArgs::Set(const String& key, const JsonData& value /* = JsonData()*/)
    {
        m_arguments[key] = value;
    }

    bool LaunchArgs::Has(const String& key) const
    {
        return m_arguments.Has(key);
    }

    void LaunchArgs::Erase(const String& key)
    {
        JPT_ASSERT(m_arguments.Has(key), "CommandLine doesn't exist \"%s\"", key.ConstBuffer());
        m_arguments.Erase(key);
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
        JsonData value;

        const size_t equalPos = argument.Find('=');

        // Flag
        if (equalPos == kInvalidIndex)
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
}