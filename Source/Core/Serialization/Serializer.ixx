// Copyright Jupiter Technologies, Inc. All Rights Reserved.

module;

#include <fstream>

export module jpt.Serializer;

import jpt.Concepts;
import jpt.TypeTraits;

export namespace jpt
{
	//template<typename T>
	//concept Serializable = Primitive<T> || 
	//	requires(T obj, std::ifstream & is, std::ofstream & os)
	//{
	//	obj.Serialize(os);
	//	obj.Deserialize(is);
	//};

	template<typename T>
		concept Serializable = requires(T obj, std::ifstream & is, std::ofstream & os)
	{
		obj.Serialize(os);
		obj.Deserialize(is);
	};
}