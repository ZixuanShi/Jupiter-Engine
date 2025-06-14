// Copyright Jupiter Technologies, Inc. All Rights Reserved.

module; 

#include "Core/Validation/Assert.h"

export module jpt.JsonData;

import jpt.Concepts;
import jpt.DynamicArray;
import jpt.HashMap;

import jpt.String;
import jpt.StringHelpers;
import jpt.ToString;

import jpt.TypeDefs;
import jpt.TypeTraits;
import jpt.Variant;

export namespace jpt
{
    class JsonData;
    using JsonArray = DynamicArray<JsonData>;
    using JsonMap   = HashMap<String, JsonData>;

    template<typename T>
    concept ValidJsonType = IsAnyOf<T, int32, float32, bool, String, JsonArray, JsonMap>;

    /** Represents a single data in Json file */
    class JsonData
    {
        using TData = Variant<int32,
                              float32,
                              bool,
                              String,
                              JsonArray,
                              JsonMap>;

    private:
        TData m_data;

    public:
        constexpr JsonData();

        template<ValidJsonType T>
        constexpr JsonData(const T& value);

        template<ValidJsonType T>
        constexpr JsonData& operator=(const T& value);

        template<ValidJsonType T> constexpr              bool Is() const { return m_data.Is<T>(); }
        template<ValidJsonType T> constexpr                T& As()       { return m_data.As<T>(); }
        template<ValidJsonType T> constexpr          const T& As() const { return m_data.As<T>(); }
        template<ValidJsonType T> constexpr operator       T& ()         { return m_data.As<T>(); }
        template<ValidJsonType T> constexpr operator const T& ()   const { return m_data.As<T>(); }
    };

    // ------------------------------------------------------------------------------------------------
    // Non-Member functions
    // ------------------------------------------------------------------------------------------------
    String ToString(const JsonArray& array);

    String ToString(const JsonData& jsonData)
    {
        if (jsonData.Is<int32>())
        {
            return jpt::ToString<String, int32>(jsonData.As<int32>());
        }
        else if (jsonData.Is<float32>())
        {
            return jpt::ToString<String, float32>(jsonData.As<float32>());
        }
        else if (jsonData.Is<bool>())
        {
            return jpt::ToString(jsonData.As<bool>());
        }
        else if (jsonData.Is<String>())
        {
            const String& str = jsonData.As<String>();
            if (str == "null")
            {
                return "null";
            }
            else
            {
                return "\"" + str + "\"";
            }
        }
        else if (jsonData.Is<JsonArray>())
        {
            return ToString(jsonData.As<JsonArray>());
        }

        JPT_ASSERT(false, "Unsupported data type in json file");
        return String();
    }

    String ToString(const JsonArray& array)
    {
        String content("[");

        for (size_t i = 0; i < array.Count(); ++i)
        {
            const JsonData& value = array[i];
            const String& str = ToString(value);

            content.Append(str);

            if (i < array.Count() - 1)
            {
                content.Append(", ");
            }
        }

        content.Append("]");
        return content;
    }

    void Recur_JsonMapToString(const JsonMap& map, String& content, uint8 depth)
    {
        // Helper function to add tabs by depth
        auto addTabs = [&content, depth]()
            {
                for (uint8 i = 0; i < depth; ++i)
                {
                    content.Append("    ");
                }
            };

        // Iterate through the map
        size_t count = 0;
        for (const auto& [key, value] : map)
        {
            addTabs();

            // Add key
            content.Append("\"");
            content.Append(key);
            content.Append("\": ");

            // If the value is a map, add a new line and recurse to next depth
            if (value.Is<JsonMap>())
            {
                content.Append("\n");
                addTabs();
                content.Append("{\n");

                Recur_JsonMapToString(value.As<JsonMap>(), content, depth + 1);

                addTabs();
                content.Append("}");
            }
            // Standard data will be added as is after the key
            else
            {
                content.Append(ToString(value));
            }

            // Add comma if it's not the last element
            ++count;
            if (count < map.Count())
            {
                content.Append(",\n");
            }
            else
            {
                content.Append("\n");
            }
        }
    }
    String ToString(const JsonMap& map)
    {
        String content("{\n");
        Recur_JsonMapToString(map, content, 1);
        content.Append("}");
        return content;
    }

    template<ValidJsonType T>
    constexpr bool operator==(const JsonData& lhs, const T& rhs)
    {
        JPT_ASSERT(lhs.Is<T>());
        return lhs.As<T>() == rhs;
    }

    constexpr bool operator==(const JsonData& lhs, const JsonData& rhs)
    {
        if (lhs.Is<int32>())
        {
            return lhs.As<int32>() == rhs.As<int32>();
        }
        else if (lhs.Is<float32>())
        {
            return lhs.As<float32>() == rhs.As<float32>();
        }
        else if (lhs.Is<bool>())
        {
            return lhs.As<bool>() == rhs.As<bool>();
        }
        else if (lhs.Is<String>())
        {
            return lhs.As<String>() == rhs.As<String>();
        }
        else if (lhs.Is<JsonArray>())
        {
            return lhs.As<JsonArray>() == rhs.As<JsonArray>();
        }
        else if (lhs.Is<JsonMap>())
        {
            return lhs.As<JsonMap>() == rhs.As<JsonMap>();
        }

        JPT_ASSERT(false, "Unsupported data type for JsonData operator==");
        return false;
    }

    // ------------------------------------------------------------------------------------------------
    // Member functions
    // ------------------------------------------------------------------------------------------------
    constexpr JsonData::JsonData()
        : m_data(String("null"))
    {
    }

    template<ValidJsonType T>
    constexpr JsonData::JsonData(const T& value)
        : m_data(value)
    {
    }

    template<ValidJsonType T>
    constexpr JsonData& JsonData::operator=(const T& value)
    {
        m_data = value;
        return *this;
    }
}