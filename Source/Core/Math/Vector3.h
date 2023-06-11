// Copyright Jupiter Technologies, Inc. All Rights Reserved.

#pragma once

namespace jpt
{
	/* Jupiter's vector 3d implementation. Templated and supports arithmetic types */
	template<typename Type>
	class Vector3
	{
	public:
		Type x = kDefaultValue<Type>;
		Type y = kDefaultValue<Type>;
		Type z = kDefaultValue<Type>;

	public:
		Vector3() = default;
		Vector3(Type value);
		Vector3(Type _x, Type _y, Type _z);
	};

	template<typename Type>
	Vector3<Type>::Vector3(Type value)
		: x(value)
		, y(value)
		, z(value)
	{
	}

	template<typename Type>
	Vector3<Type>::Vector3(Type _x, Type _y, Type _z)
		: x(_x)
		, y(_y)
		, z(_z)
	{
	}

	using FVec3 = Vector3<float>;
	using IVec3 = Vector3<int32>;
}