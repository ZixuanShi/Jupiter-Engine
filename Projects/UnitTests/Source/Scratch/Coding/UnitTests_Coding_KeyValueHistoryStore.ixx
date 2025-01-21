// Copyright Jupiter Technologies, Inc. All Rights Reserved.

module;

#include "Core/Minimal/CoreHeaders.h"

export module UnitTests_Coding_KeyValueHistoryStore;

import jpt.Constants;
import jpt.TypeDefs;
import jpt.Utilities;
import jpt.DynamicArray;
import jpt.Pair;
import jpt.HashMap;
import jpt.String;

class Store
{
public:
	using Record = jpt::Pair<uint32, int32>;
	using History = jpt::DynamicArray<Record>;

private:
	jpt::HashMap<jpt::String, History> m_data;
	uint32 m_version = 1;

public:
	void Put(const jpt::String& key, int32 value);
	int32 Get(const jpt::String& key) const;
	int32 Get(const jpt::String& key, uint32 version) const;
};

void Store::Put(const jpt::String& key, int32 value)
{
	History& history = m_data[key];
	history.Add({ m_version, value });

	JPT_LOG("Put(#%u) %s: %i", m_version, key.ConstBuffer(), value);

	++m_version;
}

int32 Store::Get(const jpt::String& key) const
{
	const auto itr = m_data.Find(key);
	if (itr == m_data.end())
	{
		JPT_LOG("Get(#%u) %s: NULL", m_version, key.ConstBuffer());
		return jpt::kInvalidValue<int32>;
	}

	const History& history = itr->second;
	const Record& record = history.Back();

	JPT_LOG("Get(#%u) %s: %i", m_version, key.ConstBuffer(), record.second);

	return record.second;
}

int32 Store::Get(const jpt::String& key, uint32 version) const
{
	JPT_ASSERT(version < m_version);

	const auto itr = m_data.Find(key);
	if (itr == m_data.end())
	{
		JPT_LOG("Get(#%u) %s: NULL", m_version, key.ConstBuffer());
		return jpt::kInvalidValue<int32>;
	}

	const History& history = itr->second;

	// Has the key been added before the first version?
	const Record& firstRecord = history.Front();
	if (version < firstRecord.first)
	{
		JPT_LOG("Get(#%u) %s: NULL", m_version, key.ConstBuffer());
		return jpt::kInvalidValue<int32>;
	}

	// Has the key been added after the last version?
	const Record& lastRecord = history.Back();
	if (lastRecord.first <= version)
	{
		JPT_LOG("Get(#%u) %s: %i", m_version, key.ConstBuffer(), lastRecord.second);
		return lastRecord.second;
	}

	// Check if the version is within the range of the history.
	for (size_t i = 0; i < history.Count() - 1; ++i)
	{
		const Record& currentRecord = history[i];
		const Record& nextRecord = history[i + 1];

		if (currentRecord.first <= version && version < nextRecord.first)
		{
			JPT_LOG("Get(#%u) %s: %i", m_version, key.ConstBuffer(), currentRecord.second);
			return currentRecord.second;
		}
	}

	JPT_LOG("Get(#%u) %s: NULL", m_version, key.ConstBuffer());
	return jpt::kInvalidValue<int32>;
}

export bool UnitTests_Coding_KeyValueHistoryStore()
{
	//Store store;

	//store.Put("key1", 1);
	//store.Put("key1", 5);
	//store.Put("key2", 10);
	//store.Put("key1", 3);
	//store.Put("key3", 10);
	//store.Put("key2", 15);
	//store.Put("key3", 10);
	//store.Put("key1", 6);
	//store.Put("key4", 1);
	//store.Get("key1");
	//store.Get("key2");
	//store.Get("key3");
	//store.Get("key1", 8);
	//store.Get("key2", 1);
	//store.Get("key2", 6);

	return true;
}