// Copyright Jupiter Technologies, Inc. All Rights Reserved.

// Inspired by 
// http://mitchnull.blogspot.ca/2012/06/c11-tuple-implementation-details-part-1.html 
// https://github.com/electronicarts/EASTL/blob/master/include/EASTL/tuple.h
// Integrated C++20 features to where it's applicable

module;

export module jpt.Tuple;

import jpt.Utilities;
import jpt.TypeTraits;

//namespace jpt_private
//{
//	// TupleLeaf
//	template<size_t I, typename ValueType, bool IsEmpty = jpt::IsEmpty<ValueType>>
//	class TupleLeaf;
//
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