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
//	// Basic building-block
//	// Each leaf has an index, so that each base-class becomes unique even if the types they contain are identical, 
//	// so we can access the nth element with a simple static_cast:
//	template<size_t I, typename ValueType, bool IsEmptyObj = jpt::IsEmptyObj<ValueType>>
//	class TupleLeaf;
//
//	template<size_t I, typename ValueType, bool IsEmptyObj>
//	void Swap(TupleLeaf<I, ValueType, IsEmptyObj>& a, TupleLeaf<I, ValueType, IsEmptyObj>& b)
//	{
//		jpt::Swap(a.Get(), b.Get());
//	}
//
//	template<size_t _I, typename _ValueType, bool _IsEmptyObj>
//	class TupleLeaf
//	{
//	private:
//		_ValueType m_value;
//
//	public:
//		TupleLeaf()
//		{
//			static_assert(!);
//		}
//	};
//}
//
//namespace jpt
//{
//	export template<typename... DataTs>
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