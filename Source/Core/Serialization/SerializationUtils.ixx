// Copyright Jupiter Technologies, Inc. All Rights Reserved.

module;

#include "Debugging/Assert.h"
#include "Debugging/Logger.h"

#include <fstream>
#include <string>

export module jpt.SerializationUtils;

import jpt.String;
import jpt.StringUtils;
import jpt.Optional;
import jpt.Serializer;

import jpt.File.Path;

export namespace jpt
{
	template<typename T>
	concept Serializable = requires(T obj, jpt::Serializer& serializer)
	{
		obj.Serialize(serializer);
		obj.Deserialize(serializer);
	};

	template<Serializable T>
	void Serialize(const T& obj, jpt::Serializer& serializer)
	{
		obj.Serialize(serializer);
	}

	template<Serializable T>
	void Deserialize(T& obj, jpt::Serializer& serializer)
	{
		obj.Deserialize(serializer);
	}

	template<typename T> requires(!Serializable<T>)
	void Serialize(const T& obj, jpt::Serializer& serializer)
	{
		serializer.Write(reinterpret_cast<const char*>(&obj), sizeof(T));
	}

	template<typename T> requires(!Serializable<T>)
	void Deserialize(T& obj, jpt::Serializer& serializer)
	{
		serializer.Read(reinterpret_cast<char*>(&obj), sizeof(T));
	}
}