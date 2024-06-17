// Copyright Jupiter Technologies, Inc. All Rights Reserved.

export module jpt.SerializationUtils;

import jpt.Serializer;

export namespace jpt
{
	template<typename T>
	concept Serializable = requires(T obj, Serializer& serializer)
	{
		obj.Serialize(serializer);
		obj.Deserialize(serializer);
	};

	template<Serializable T>
	void Serialize(const T& obj, Serializer& serializer)
	{
		obj.Serialize(serializer);
	}

	template<Serializable T>
	void Deserialize(T& obj, Serializer& serializer)
	{
		obj.Deserialize(serializer);
	}

	template<typename T> requires(!Serializable<T>)
	void Serialize(const T& obj, Serializer& serializer)
	{
		serializer.Write(reinterpret_cast<const char*>(&obj), sizeof(T));
	}

	template<typename T> requires(!Serializable<T>)
	void Deserialize(T& obj, Serializer& serializer)
	{
		serializer.Read(reinterpret_cast<char*>(&obj), sizeof(T));
	}
}