// Copyright Jupiter Technologies, Inc. All Rights Reserved.

module;

#include "Debugging/Assert.h"

export module jpt.JsonObject;

import jpt.DynamicArray;
import jpt.HashMap;
import jpt.String;
import jpt.TypeDefs;
import jpt.TypeTraits;
import jpt.Utilities;
import jpt.Variant;

export namespace jpt
{
	using JsonSingleData = Variant<int32, float32, bool, String>;
	using JsonData = Variant<int32, float32, bool, String, DynamicArray<JsonSingleData>>;

	/** Represents a scope in Json file, also known as a map that stores key-value pairs in the same scope layer 
		To inqury data in a subset of map.  */
	class JsonObject
	{
	private:
		HashMap<String, JsonData> m_data;		/**< Single data */
		HashMap<String, JsonObject> m_subMaps;	/**< Nested map data */

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

	private:
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
}