// Copyright Jupiter Technologies, Inc. All Rights Reserved.

module;

#include "Debugging/Assert.h"

#include <string>

export module jpt.Variant;

import jpt.Allocator;
import jpt.Byte;
import jpt.Concepts;
import jpt.TypeTraits;
import jpt.TypeDefs;
import jpt.Math;
import jpt.Utilities;

export namespace jpt
{
	template<typename...TArgs>
	class Variant
	{
		using TIndex = uint8;

		static constexpr size_t kMaxTypeSize = Max(sizeof(TArgs)...);	/**< The biggest size amoung TArgs */
		static constexpr TIndex kTypesCount  = sizeof...(TArgs);	    /**< Count of TArgs */

		static_assert(kTypesCount > 0, "Variant must have at least one type");

	private:
		Byte m_buffer[kMaxTypeSize];       /**< The buffer to store the value of the Variant. Sized by the biggest type */
		TIndex m_currentIndex = kInvalidValue<TIndex>;  /**< The current using index in m_typeInfos */

	public:
		constexpr Variant() = default;
		constexpr ~Variant();
		constexpr Variant(const Variant& other);
		constexpr Variant(Variant&& other) noexcept;
		constexpr Variant& operator=(const Variant& other);
		constexpr Variant& operator=(Variant&& other) noexcept;

		/** Assign value to this variant */
		template<typename T> constexpr Variant(const T& value) requires IsAnyOf<T, TArgs...>;
		template<typename T> constexpr Variant(T&& value)      requires IsAnyOf<T, TArgs...>;
		template<typename T> constexpr Variant& operator=(const T& value) requires IsAnyOf<T, TArgs...>;
		template<typename T> constexpr Variant& operator=(T&& value)      requires IsAnyOf<T, TArgs...>;

		/** @return		Reference to the current buffer data that casted to given T */
		template<typename T> constexpr       T& As()       requires IsAnyOf<T, TArgs...>;
		template<typename T> constexpr const T& As() const requires IsAnyOf<T, TArgs...>;

		/** @return		true if this variant's current assigned type is same as the T */
		template<typename T> constexpr bool Is() const requires IsAnyOf<T, TArgs...>;

	private:
		template<typename T> constexpr void Construct(const T& value) requires IsAnyOf<T, TArgs...>;
		template<typename T> constexpr void Construct(T&& value)      requires IsAnyOf<T, TArgs...>;

		template<typename TCurrent, typename ...TRest>
		constexpr void Destruct();

		/** @return		The index of the TypeToFind in TArgs */
		template<typename TypeToFind, typename TCurrent, typename ...TRest>
		constexpr TIndex GetIndexOfType() const requires IsAnyOf<TypeToFind, TArgs...>;

		template<typename TCurrent, typename ...TRest> constexpr void CopyData(const Variant& other);
		template<typename TCurrent, typename ...TRest> constexpr void MoveData(Variant&& other);
	};

	template<typename ...TArgs>
	constexpr Variant<TArgs...>::~Variant()
	{
		Destruct<TArgs...>();
	}

	template<typename ...TArgs>
	constexpr Variant<TArgs...>::Variant(const Variant& other)
	{
		CopyData<TArgs...>(other);
	}

	template<typename ...TArgs>
	constexpr Variant<TArgs...>::Variant(Variant&& other) noexcept
	{
		MoveData<TArgs...>(Move(other));
	}

	template<typename ...TArgs>
	constexpr Variant<TArgs...>& Variant<TArgs...>::operator=(const Variant& other)
	{
		if (this != &other)
		{
			Destruct<TArgs...>();
			CopyData<TArgs...>(other);
		}

		return *this;
	}

	template<typename ...TArgs>
	constexpr Variant<TArgs...>& Variant<TArgs...>::operator=(Variant&& other) noexcept
	{
		if (this != &other)
		{
			Destruct<TArgs...>();
			MoveData<TArgs...>(Move(other));
		}

		return *this;
	}

	template<typename ...TArgs>
	template<typename T>
	constexpr Variant<TArgs...>::Variant(const T& value) requires IsAnyOf<T, TArgs...>
	{
		Construct<T>(value);
	}

	template<typename ...TArgs>
	template<typename T>
	constexpr Variant<TArgs...>::Variant(T&& value) requires IsAnyOf<T, TArgs...>
	{
		Construct<T>(Move(value));
	}

	template<typename ...TArgs>
	template<typename T>
	constexpr Variant<TArgs...>& Variant<TArgs...>::operator=(const T& value) requires IsAnyOf<T, TArgs...>
	{
		Destruct<TArgs...>();
		Construct<T>(value);
		return *this;
	}

	template<typename ...TArgs>
	template<typename T>
	constexpr Variant<TArgs...>& Variant<TArgs...>::operator=(T&& value) requires IsAnyOf<T, TArgs...>
	{
		Destruct<TArgs...>();
		Construct<T>(Move(value));
		return *this;
	}

	template<typename ...TArgs>
	template<typename T>
	constexpr T& Variant<TArgs...>::As() requires IsAnyOf<T, TArgs...>
	{
		JPT_ASSERT(Is<T>(), "Variant should be assigned to the given type T before calling As<T>() ");

		return reinterpret_cast<T&>(m_buffer);
	}

	template<typename ...TArgs>
	template<typename T>
	constexpr const T& Variant<TArgs...>::As() const requires IsAnyOf<T, TArgs...>
	{
		JPT_ASSERT(Is<T>(), "Variant should be assigned to the given type T before calling As<T>() ");

		return reinterpret_cast<const T&>(m_buffer);
	}

	template<typename ...TArgs>
	template<typename T>
	constexpr bool Variant<TArgs...>::Is() const requires IsAnyOf<T, TArgs...>
	{
		if (m_currentIndex == kInvalidValue<TIndex>)
		{
			return false;
		}

		return m_currentIndex == GetIndexOfType<T, TArgs...>();
	}

	template<typename ...TArgs>
	template<typename T>
	constexpr void Variant<TArgs...>::Construct(const T& value) requires IsAnyOf<T, TArgs...>
	{
		Allocator<T>::Construct(reinterpret_cast<T*>(&m_buffer), value);
		m_currentIndex = GetIndexOfType<T, TArgs...>();
	}

	template<typename ...TArgs>
	template<typename T>
	constexpr void Variant<TArgs...>::Construct(T&& value) requires IsAnyOf<T, TArgs...>
	{
		Allocator<T>::Construct(reinterpret_cast<T*>(&m_buffer), Move(value));
		m_currentIndex = GetIndexOfType<T, TArgs...>();
	}

	template<typename ...TArgs>
	template<typename TCurrent, typename ...TRest>
	constexpr void Variant<TArgs...>::Destruct()
	{
		if (m_currentIndex == kInvalidValue<TIndex>)
		{
			return;
		}

		if (m_currentIndex == kTypesCount - sizeof...(TRest) - 1)
		{
			if constexpr (!IsTriviallyDestructible<TCurrent>)
			{
				Allocator<TCurrent>::Destruct(reinterpret_cast<TCurrent*>(&m_buffer));
			}

			m_currentIndex = kInvalidValue<TIndex>;
		}

		if constexpr (sizeof...(TRest) > 0)
		{
			Destruct<TRest...>();
		}
	}

	template<typename ...TArgs>
	template<typename TypeToFind, typename TCurrent, typename ...TRest>
	constexpr Variant<TArgs...>::TIndex Variant<TArgs...>::GetIndexOfType() const requires IsAnyOf<TypeToFind, TArgs...>
	{
		if constexpr (IsSameType<TypeToFind, TCurrent>)
		{
			return kTypesCount - sizeof...(TRest) - 1;
		}
		else
		{
			return GetIndexOfType<TypeToFind, TRest...>();
		}
	}

	template<typename ...TArgs>
	template<typename TCurrent, typename ...TRest>
	constexpr void Variant<TArgs...>::CopyData(const Variant& other)
	{
		if (other.m_currentIndex == kInvalidValue<TIndex>)
		{
			return;
		}

		if (other.m_currentIndex == kTypesCount - sizeof...(TRest) - 1)
		{
			Construct<TCurrent>(other.As<TCurrent>());
		}

		if constexpr (sizeof...(TRest) > 0)
		{
			CopyData<TRest...>(other);
		}
	}

	template<typename ...TArgs>
	template<typename TCurrent, typename ...TRest>
	constexpr void Variant<TArgs...>::MoveData(Variant&& other)
	{
		if (other.m_currentIndex == kInvalidValue<TIndex>)
		{
			return;
		}

		if (other.m_currentIndex == kTypesCount - sizeof...(TRest) - 1)
		{
			Construct<TCurrent>(Move(other.As<TCurrent>()));
			other.m_currentIndex = kInvalidValue<TIndex>;
		}

		if constexpr (sizeof...(TRest) > 0)
		{
			MoveData<TRest...>(Move(other));
		}
	}
}