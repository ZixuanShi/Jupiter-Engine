// Copyright Jupiter Technologies, Inc. All Rights Reserved.

module; 

#include "Debugging/Assert.h"

export module jpt.JsonData;

import jpt.Variant;
import jpt.TypeDefs;
import jpt.DynamicArray;
import jpt.String;
import jpt.StringUtils;
import jpt.ToString;

export namespace jpt
{
	using TJsonData = Variant<int32, float32, bool, String, DynamicArray<Variant<int32, float32, bool, String>>>;

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

		String ToString() const;
	};

	template<typename T>
	JsonData::JsonData(const T& value)
		: data(value)
	{
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

		JPT_ASSERT(false, "Unsupported data type in json file");
		return String();
	}
}