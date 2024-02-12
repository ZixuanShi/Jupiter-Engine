// Copyright Jupiter Technologies, Inc. All Rights Reserved.

// Warning: Client ixx file MUST #include <typeinfo> to use typeid. Otherwise error C2027: use of undefined type 'type_info'

module;

#include "Core/Minimal/Macros.h"
#include "Debugging/Assert.h"
#include "Debugging/Logger.h"

export module jpt.Variant;

import jpt.Allocator;
import jpt.Byte;
import jpt.TypeDefs;
import jpt.TypeTraits;
import jpt.Utilities;
import jpt.Math;
import jpt.Pair;

export namespace jpt
{
	template<typename...TArgs>
	class Variant
	{
		using TypeInfo = Pair<size_t, const char*>; /**< size_t: hash code, const char*: type name */

		static constexpr size_t kMaxTypeSize = Max(sizeof(TArgs)...);	/**< The biggest size amoung TArgs */
		static constexpr size_t kTypesCount  = sizeof...(TArgs);	    /**< Count of TArgs */

	private:
		Byte     m_buffer[kMaxTypeSize];    /**< The buffer to store the value of the Variant. Sized by the biggest type */
		TypeInfo m_typeInfos[kTypesCount];  /**< Stores info about TArgs */

		size_t m_currentIndex = kInvalidValue<size_t>;  /**< The current using index in m_typeInfos */

	public:
		constexpr Variant();

		template<typename T>
		constexpr Variant(const T& value);

		template<typename T>
		constexpr Variant& operator=(const T& value);

		void Debug();

	private:
		template<typename T>
		constexpr void Construct(const T& value);

		constexpr void Destruct();

		template<typename T>
		constexpr size_t FindIndexOfType(const T& value) const;

		// Recursive initializing type info. Should be called only once
		template<typename TFinal>
		constexpr void InitFinalTypeInfo();

		template<typename TCurrent, typename ...TRest>
		constexpr void InitTypeInfos();
	};

	template<typename ...TArgs>
	constexpr Variant<TArgs...>::Variant()
	{
		InitTypeInfos<TArgs...>();
	}

	template<typename ...TArgs>
	template<typename T>
	constexpr Variant<TArgs...>::Variant(const T& value)
	{
		InitTypeInfos<TArgs...>();
		Construct<T>(value);
	}

	template<typename ...TArgs>
	template<typename T>
	constexpr Variant<TArgs...>& Variant<TArgs...>::operator=(const T& value)
	{
		Destruct();
		Construct<T>(value);
		return *this;
	}

	template<typename ...TArgs>
	template<typename T>
	constexpr void Variant<TArgs...>::Construct(const T& value)
	{
		static_assert(IsAnyOf<T, TArgs...>, "T is not in Variant");

		Allocator<T>::Construct(reinterpret_cast<T*>(&m_buffer), value);
		m_currentIndex = FindIndexOfType(value);
	}

	template<typename ...TArgs>
	constexpr void Variant<TArgs...>::Destruct()
	{
		if (m_currentIndex == kInvalidValue<size_t>)
		{
			return;
		}

		// Call the destructor of the current type

	}

	template<typename ...TArgs>
	void Variant<TArgs...>::Debug()
	{
		for (size_t i = 0; i < kTypesCount; ++i)
		{
			JPT_LOG("Index: %zu, HashCode: %zu, Name: %s", i, m_typeInfos[i].first, m_typeInfos[i].second);
		}
	}

	template<typename ...TArgs>
	template<typename T>
	constexpr size_t Variant<TArgs...>::FindIndexOfType(const T& value) const
	{
		static_assert(IsAnyOf<T, TArgs...>, "T is not in Variant");

		const size_t hashCode = typeid(value).hash_code();
		for (size_t i = 0; i < kTypesCount; ++i)
		{
			if (m_typeInfos[i].first == hashCode)
			{
				return i;
			}
		}

		return kInvalidValue<size_t>;
	}

	template<typename ...TArgs>
	template<typename TFinal>
	constexpr void Variant<TArgs...>::InitFinalTypeInfo()
	{
		m_typeInfos[kTypesCount - 1].first  = typeid(TFinal).hash_code();
		m_typeInfos[kTypesCount - 1].second = typeid(TFinal).name();
	};

	template<typename ...TArgs>
	template<typename TCurrent, typename ...TRest>
	constexpr void Variant<TArgs...>::InitTypeInfos()
	{
		m_typeInfos[kTypesCount - sizeof...(TRest) - 1].first  = typeid(TCurrent).hash_code();
		m_typeInfos[kTypesCount - sizeof...(TRest) - 1].second = typeid(TCurrent).name();

		if constexpr (sizeof...(TRest) > 1)
		{
			InitTypeInfos<TRest...>();
		}
		else
		{
			InitFinalTypeInfo<TRest...>();
		}
	}
}