// Copyright Jupiter Technologies, Inc. All Rights Reserved.

module;

#include "Core/Minimal/Macros.h"
#include "Debugging/Assert.h"

export module jpt.Variant;

import jpt.Allocator;
import jpt.Byte;
import jpt.TypeDefs;
import jpt.TypeTraits;
import jpt.Utilities;

export namespace jpt
{
	template<typename...TArgs>
	class Variant
	{
	private:
		Byte* m_pBuffer = nullptr;

	public:
		constexpr Variant() = default;

		template<typename T>
		constexpr Variant(const T& value);

		template<typename T>
		constexpr Variant& operator=(const T& value);

	private:
		template<typename T>
		void Construct(const T& value);

		void Destruct();
	};

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
		JPT_IGNORE(value);
		static_assert(IsAnyOf<T, TArgs...>, "T is not in variant");
		//m_pBuffer = Allocator::Allocate(sizeof(T));
		//new (m_pBuffer) T(value);	
	}

	template<typename ...TArgs>
	void Variant<TArgs...>::Destruct()
	{
		if (m_pBuffer)
		{
			//Allocator::Deallocate(m_pBuffer);
			//m_pBuffer = nullptr;
		}
	}
}