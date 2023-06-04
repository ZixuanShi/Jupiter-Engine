// Copyright Jupiter Technologies, Inc. All Rights Reserved.

#pragma once

namespace jpt
{
	template<class TypeA, class TypeB>
	struct pair
	{
	public:
		TypeA first;
		TypeB second;

	public:
		pair() = default;
		pair(const pair&) = default;
		pair(pair&&) noexcept = default;
		pair& operator=(const pair&) = default;
		pair& operator=(pair&&) noexcept = default;
		pair(const TypeA& inFirst, const TypeB& inSecond);
		pair(TypeA&& inFirst, TypeB&& inSecond);
	};

	template<class TypeA, class TypeB>
	inline pair<TypeA, TypeB>::pair(const TypeA& inFirst, const TypeB& inSecond)
		: first(inFirst)
		, second(inSecond)
	{
	}

	template<class TypeA, class TypeB>
	inline pair<TypeA, TypeB>::pair(TypeA&& inFirst, TypeB&& inSecond)
		: first(jpt::move(inFirst))
		, second(jpt::move(inSecond))
	{
	}

	template<class TypeA, class TypeB>
	pair<TypeA, TypeB> make_pair(const TypeA& inFirst, const TypeB& inSecond)
	{
		return jpt::pair<TypeA, TypeB>(inFirst, inSecond);
	}

	template<class TypeA, class TypeB>
	pair<TypeA, TypeB> make_pair(TypeA&& inFirst, TypeB&& inSecond)
	{
		return jpt::pair<TypeA, TypeB>(inFirst, inSecond);
	}
}

