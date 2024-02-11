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
		using TypeInfo = Pair<size_t, const char*>; // size_t: hash code, const char*: type name

	private:
		static constexpr size_t kMaxTypeSize = Max(sizeof(TArgs)...);	/**< The biggest size amoung TArgs */
		static constexpr size_t kTypesCount  = sizeof...(TArgs);	    /**< Count of TArgs */

		Byte m_buffer[kMaxTypeSize];	/**< The buffer to store the value of the Variant. Sized by the biggest type */
		TypeInfo m_typeInfos[kTypesCount];	/**<  */

		size_t m_currentIndex = kInvalidValue<size_t>;	/**< The index of the current using type in the Variant */

	public:
		constexpr Variant();

		template<typename T>
		constexpr Variant(const T& value);

		template<typename T>
		constexpr Variant& operator=(const T& value);

		void Debug();

	private:
		template<typename T>
		void Construct(const T& value);

		void Destruct();

		template<typename TFinal>
		constexpr void SetFinalTypeHashCode();

		template<typename TCurrent, typename ...TRest>
		constexpr void SetTypeHashCode();
	};

	template<typename ...TArgs>
	constexpr Variant<TArgs...>::Variant()
	{
		SetTypeHashCode<TArgs...>();
	}

	template<typename ...TArgs>
	template<typename T>
	constexpr Variant<TArgs...>::Variant(const T& value)
	{
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
	void Variant<TArgs...>::Construct(const T& value)
	{
		static_assert(IsAnyOf<T, TArgs...>, "T is not in Variant");

		JPT_IGNORE(value);
	}

	template<typename ...TArgs>
	void Variant<TArgs...>::Destruct()
	{
		if (m_currentIndex == kInvalidValue<size_t>)
		{
			return;
		}

		
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
	template<typename TFinal>
	constexpr void Variant<TArgs...>::SetFinalTypeHashCode()
	{
		m_typeInfos[kTypesCount - 1] = { typeid(TFinal).hash_code(), typeid(TFinal).name() };
	}

	template<typename ...TArgs>
	template<typename TCurrent, typename ...TRest>
	constexpr void Variant<TArgs...>::SetTypeHashCode()
	{
		m_typeInfos[kTypesCount - sizeof...(TRest) - 1] = { typeid(TCurrent).hash_code(), typeid(TCurrent).name() };

		if constexpr (sizeof...(TRest) > 1)
		{
			SetTypeHashCode<TRest...>();
		}
		else
		{
			SetFinalTypeHashCode<TRest...>();
		}
	}
}