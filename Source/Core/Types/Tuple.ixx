// Copyright Jupiter Technologies, Inc. All Rights Reserved.

// Inspired by 
// http://mitchnull.blogspot.ca/2012/06/c11-tuple-implementation-details-part-1.html 
// https://github.com/electronicarts/EASTL/blob/master/include/EASTL/tuple.h
// Integrated C++20 features to where it's applicable

module;

export module jpt.Tuple;

import jpt.Utilities;

//namespace jpt_private
//{
//	template<size_t ...Indices, typename... DataTs>
//	struct TupleImpl<integer_sequence<size_t, Indices...>, DataTs...> :public TupleLeaf<Indices, DataTs>...
//	{
//
//	}
//}
//
//namespace jpt
//{
//	template<typename... DataTs>
//	class Tuple;
//
//	export template<typename DataT, typename... DataTs>
//	class Tuple<DataT, DataTs...>
//	{
//	public:
//		using Impl = jpt_private::TupleImpl<make_index_sequence<sizeof...(DataTs) + 1>, DataT, DataTs...> Impl;
//
//
//	private:
//		Impl m_impl;
//
//	public:
//
//	};
//}