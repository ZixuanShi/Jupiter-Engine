// Copyright Jupiter Technologies, Inc. All Rights Reserved.

module; 

#include "Debugging/Assert.h"

export module jpt.Json.Data;

import jpt.Concepts;
import jpt.DynamicArray;
import jpt.HashMap;
import jpt.String;
import jpt.String.Helpers;
import jpt.ToString;
import jpt.TypeDefs;
import jpt.TypeTraits;
import jpt.Variant;

namespace jpt
{
	class JsonData;
	export using JsonArray = DynamicArray<JsonData>;

	/** Encapsulates a hashmap that Has Json data within a scope */
	export class JsonMap
	{
	private:
		using TMap = HashMap<String, JsonData>;

	private:
		TMap m_map;

	public:
		constexpr TMap::Iterator      begin()        { return m_map.begin();  }
		constexpr TMap::Iterator      end()          { return m_map.end();    }
		constexpr TMap::ConstIterator begin()  const { return m_map.begin();  }
		constexpr TMap::ConstIterator end()    const { return m_map.end();    }
		constexpr TMap::ConstIterator cbegin() const { return m_map.cbegin(); }
		constexpr TMap::ConstIterator cend()   const { return m_map.cend();   }
		constexpr Index Count() const { return m_map.Count(); }
		constexpr bool operator==(const JsonMap& other) const { return m_map == other.m_map; }

		template<typename T>
		constexpr void Set(const String& key, const T& value)
		{
			m_map[key] = value;
		}

		constexpr JsonData& operator[](const String& key)
		{
			return m_map[key];
		}

		constexpr const JsonData& operator[](const String& key) const
		{
			JPT_ASSERT(m_map.Has(key), "Couldn't find key \"%s\" within current Json scope", key.ConstBuffer());
			return m_map[key];
		}

		constexpr bool Has(const String& key) const
		{
			return m_map.Has(key);
		}

		constexpr void Reserve(Index count)
		{
			m_map.Reserve(count);
		}
	};

	template<typename T>
	concept ValidType = IsAnyOf<T, int32, float32, bool, String, JsonArray, JsonMap>;

	/** Represents a single data in Json file */
	export class JsonData
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
		constexpr JsonData() = default;

		template<ValidType T>
		constexpr JsonData(const T& value);

		template<ValidType T>
		constexpr JsonData& operator=(const T& value);

		template<ValidType T> constexpr              bool Is() const { return m_data.Is<T>(); }
		template<ValidType T> constexpr                T& As() 	     { return m_data.As<T>(); }
		template<ValidType T> constexpr          const T& As() const { return m_data.As<T>(); }
		template<ValidType T> constexpr operator       T& ()         { return m_data.As<T>(); }
		template<ValidType T> constexpr operator const T& ()   const { return m_data.As<T>(); }

		template<ValidType T>
		constexpr bool operator==(const T& other) const;

		constexpr bool operator==(const JsonData& other) const;

		constexpr String ToString() const;
	};

	template<ValidType T>
	constexpr JsonData::JsonData(const T& value)
		: m_data(value)
	{
	}

	template<ValidType T>
	constexpr JsonData& JsonData::operator=(const T& value)
	{
		m_data = value;
		return *this;
	}

	template<ValidType T>
	constexpr bool JsonData::operator==(const T& data) const
	{
		JPT_ASSERT(Is<T>());
		return m_data.As<T>() == data;
	}

	constexpr bool JsonData::operator==(const JsonData& other) const
	{
		if (Is<int32>())
		{
			return As<int32>() == other.As<int32>();
		}
		else if (Is<float32>())
		{
			return As<float32>() == other.As<float32>();
		}
		else if (Is<bool>())
		{
			return As<bool>() == other.As<bool>();
		}
		else if (Is<String>())
		{
			return As<String>() == other.As<String>();
		}
		else if (Is<JsonArray>())
		{
			return As<JsonArray>() == other.As<JsonArray>();
		}
		else if (Is<JsonMap>())
		{
			return As<JsonMap>() == other.As<JsonMap>();
		}

		JPT_ASSERT(false, "Unsupported data type for JsonData operator==");
		return false;
	}

	constexpr void Recur_JsonMapToString(const JsonMap& map, String& content, uint8 depth)
	{
		// Helper function to add tabs by depth
		auto addTabs = [&content, depth]()
		{
			for (uint8 i = 0; i < depth; ++i)
			{
				content.Append("\t");
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
				content.Append(value.ToString());
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
	export constexpr String ToString(const JsonMap& map)
	{
		String content("{\n");
		Recur_JsonMapToString(map, content, 1);
		content.Append("}");
		return content;
	}

	constexpr String JsonData::ToString() const
	{
		if (m_data.Is<int32>())
		{
			return jpt::ToString(m_data.As<int32>());
		}
		else if (m_data.Is<float32>())
		{
			return jpt::ToString(m_data.As<float32>());
		}
		else if (m_data.Is<bool>())
		{
			return jpt::ToString(m_data.As<bool>());
		}
		else if (m_data.Is<String>())
		{
			const String& str = m_data.As<String>();
			if (str == "null")
			{
				return "null";
			}
			else
			{
				return "\"" + str + "\"";
			}
		}
		else if (m_data.Is<JsonArray>())
		{
			return jpt::ToString(m_data.As<JsonArray>());
		}

		JPT_ASSERT(false, "Unsupported data type in json file");
		return String();
	}
}