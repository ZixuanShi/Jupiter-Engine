// Copyright Jupiter Technologies, Inc. All Rights Reserved.

module;

#include "Debugging/Assert.h"

export module jpt.JsonObject;

import jpt.HashMap;
import jpt.Variant;
import jpt.String;
import jpt.StringUtils;
import jpt.JsonData;
import jpt.TypeDefs;

export namespace jpt
{
	/** Represents a scope in Json file, also known as a map that stores key-value pairs in the same scope layer 
		To inqury data in a subset of map.  */
	class JsonObject
	{
	private:
		HashMap<String, JsonData> m_data;		/**< Single data */

	public:
		/** Reads an exisiting value */
		template<typename T>
		const T& Get(const String& key) const;

		/** Adds a new value. Will fail assertion if the key already exists */
		template<typename T>
		void Add(const String& key, const T& value);

		/** Updates an existing value. Will fail assertion if the key doesn't exist */
		template<typename T>
		void Update(const String& key, const T& value);

		String ToString() const;
	};

	template<typename T>
	const T& JsonObject::Get(const String& key) const
	{
		JPT_ASSERT(m_data.Contains(key), "Key not found in JsonFile");
		return m_data[key].As<T>();
	}

	template<typename T>
	void JsonObject::Add(const String& key, const T& value)
	{
		JPT_ASSERT(!m_data.Contains(key), "Key already exists in JsonFile");
		m_data.Add(key, value);
	}

	template<typename T>
	void JsonObject::Update(const String& key, const T& value)
	{
		JPT_ASSERT(m_data.Contains(key), "Key not found in JsonFile");
		m_data[key] = value;
	}

	String JsonObject::ToString() const
	{
		String str;
		str.Append("{\n");

		size_t count = 0;
		for (const Pair<String, JsonData>& pair : m_data)
		{
			str.Append("\t\"");
			str.Append(pair.first);
			str.Append("\": ");
			str.Append(pair.second.ToString());

			++count;
			if (count < m_data.Count())
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
}