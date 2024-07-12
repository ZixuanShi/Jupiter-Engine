// Copyright Jupiter Technologies, Inc. All Rights Reserved.

module; 

#include "Debugging/Assert.h"

export module jpt.JsonData;

import jpt.Concepts;
import jpt.DynamicArray;
import jpt.HashMap;
import jpt.String;
import jpt.StringUtils;
import jpt.ToString;
import jpt.TypeDefs;
import jpt.TypeTraits;
import jpt.Variant;

export namespace jpt
{
	class JsonData;
	using JsonArray = DynamicArray<JsonData>;

	/** Encapsulates a hashmap that contains Json data within a scope */
	class JsonMap
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

		constexpr const JsonData& operator[](const String& key) const
		{
			JPT_ASSERT(m_map.Contains(key));
			return m_map[key];
		}
	};

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
		JsonData() = default;

		template<typename T>
		JsonData(const T& value);

		template<typename T>
		bool Is() const { return m_data.Is<T>(); }

		template<typename T>
		const T& As() const { return m_data.As<T>(); }

		template<typename T>
		JsonData& operator=(const T& value);

		bool operator==(const JsonData& other) const;

		String ToString() const;
	};

	template<typename T>
	JsonData::JsonData(const T& value)
		: m_data(value)
	{
	}

	template<typename T>
	JsonData& JsonData::operator=(const T& value)
	{
		m_data = value;
		return *this;
	}

	bool JsonData::operator==(const JsonData& other) const
	{
		if (m_data.Is<int32>())
		{
			return m_data.As<int32>() == other.m_data.As<int32>();
		}
		else if (m_data.Is<float32>())
		{
			return m_data.As<float32>() == other.m_data.As<float32>();
		}
		else if (m_data.Is<bool>())
		{
			return m_data.As<bool>() == other.m_data.As<bool>();
		}
		else if (m_data.Is<String>())
		{
			return m_data.As<String>() == other.m_data.As<String>();
		}
		else if (m_data.Is<JsonArray>())
		{
			return m_data.As<JsonArray>() == other.m_data.As<JsonArray>();
		}
		else if (m_data.Is<JsonMap>())
		{
			return m_data.As<JsonMap>() == other.m_data.As<JsonMap>();
		}

		JPT_ASSERT(false, "Unsupported data type for JsonData operator==");
		return false;
	}

	String JsonData::ToString() const
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
		else if (m_data.Is<JsonMap>())
		{
			String str("\n{\n");

			const JsonMap& map = m_data.As<JsonMap>();
			size_t count = 0;
			for (const auto& [key, value] : map)
			{
				str.Append("\t\"");
				str.Append(key);
				str.Append("\": ");
				str.Append(value.ToString());

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

		JPT_ASSERT(false, "Unsupported data type in json file");
		return String();
	}
}