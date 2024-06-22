// Copyright Jupiter Technologies, Inc. All Rights Reserved.

module; 

#include "Debugging/Assert.h"

export module jpt.JsonData;

import jpt.Variant;
import jpt.TypeDefs;
import jpt.DynamicArray;
import jpt.HashMap;
import jpt.String;
import jpt.StringUtils;
import jpt.ToString;

export namespace jpt
{
	class JsonData;

	using JsonArray = DynamicArray<JsonData>;
	using JsonMap = HashMap<String, JsonData>;
	using TJsonData = Variant<int32, 
                             float32,
                             bool, 
                             String, 
                             JsonArray,
                             JsonMap>;

	class JsonData
	{
	private:
		TJsonData data;

	public:
		JsonData() = default;

		template<typename T>
		JsonData(const T& value);

		template<typename T>
		bool Is() const { return data.Is<T>(); }

		template<typename T>
		const T& As() const { return data.As<T>(); }

		bool operator==(const JsonData& other) const;

		String ToString() const;
	};

	template<typename T>
	JsonData::JsonData(const T& value)
		: data(value)
	{
	}

	bool JsonData::operator==(const JsonData& other) const
	{
		if (data.Is<int32>())
		{
			return data.As<int32>() == other.data.As<int32>();
		}
		else if (data.Is<float32>())
		{
			return data.As<float32>() == other.data.As<float32>();
		}
		else if (data.Is<bool>())
		{
			return data.As<bool>() == other.data.As<bool>();
		}
		else if (data.Is<String>())
		{
			return data.As<String>() == other.data.As<String>();
		}
		else if (data.Is<DynamicArray<JsonData>>())
		{
			return data.As<DynamicArray<JsonData>>() == other.data.As<DynamicArray<JsonData>>();
		}
		else if (data.Is<HashMap<String, JsonData>>())
		{
			return data.As<HashMap<String, JsonData>>() == other.data.As<HashMap<String, JsonData>>();
		}

		JPT_ASSERT(false, "Unsupported data type for JsonData operator==");
		return false;
	}

	String JsonData::ToString() const
	{
		if (data.Is<int32>())
		{
			return jpt::ToString(data.As<int32>());
		}
		else if (data.Is<float32>())
		{
			return jpt::ToString(data.As<float32>());
		}
		else if (data.Is<bool>())
		{
			return jpt::ToString(data.As<bool>());
		}
		else if (data.Is<String>())
		{
			const String& str = data.As<String>();
			if (str == "null")
			{
				return "null";
			}
			else
			{
				return "\"" + str + "\"";
			}
		}
		else if (data.Is<DynamicArray<JsonData>>())
		{
			return jpt::ToString(data.As<DynamicArray<JsonData>>());
		}
		else if (data.Is<HashMap<String, JsonData>>())
		{
			String str("\n{\n");

			size_t count = 0;
			for (const auto& [key, value] : data.As<HashMap<String, JsonData>>())
			{
				str.Append("\t\"");
				str.Append(key);
				str.Append("\": ");
				str.Append(value.ToString());

				++count;
				if (count < data.As<HashMap<String, JsonData>>().Count())
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