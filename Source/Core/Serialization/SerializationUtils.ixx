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
import jpt.File.Path;

export namespace jpt
{
	template<typename T>
	concept Serializable = requires(T obj, std::fstream& stream)
	{
		obj.Serialize(stream);
		obj.Deserialize(stream);
	};

	template<Serializable T>
	void Serialize(const T& obj, std::fstream& stream)
	{
		obj.Serialize(stream);
	}

	template<Serializable T>
	void Deserialize(T& obj, std::fstream& stream)
	{
		obj.Deserialize(stream);
	}

	template<typename T> requires(!Serializable<T>)
		void Serialize(const T& obj, std::fstream& stream)
	{
		stream.write(reinterpret_cast<const char*>(&obj), sizeof(T));
	}

	template<typename T> requires(!Serializable<T>)
		void Deserialize(T& obj, std::fstream& stream)
	{
		stream.read(reinterpret_cast<char*>(&obj), sizeof(T));
	}
}