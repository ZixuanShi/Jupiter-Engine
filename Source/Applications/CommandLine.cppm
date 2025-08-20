// Copyright Jupiter Technologies, Inc. All Rights Reserved.

module;

#include "Core/Minimal/Utilities.h"
#include "Core/Validation/Assert.h"

export module jpt.CommandLine;

import jpt.String;
import jpt.TypeDefs;
import jpt.Utilities;

import jpt.JsonData;

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