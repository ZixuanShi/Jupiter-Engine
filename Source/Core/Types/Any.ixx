// Copyright Jupiter Technologies, Inc. All Rights Reserved.

module;

#include "Core/Minimal/Macros.h"
#include "Debugging/Assert.h"

#include <typeinfo>

export module jpt.Any;

import jpt.Byte;
import jpt.Constants;
import jpt.TypeTraits;
import jpt.Utilities;
import jpt.Function;

namespace jpt
{
	export class Any
	{
	private:
		Byte* m_pBuffer = nullptr;

		Function<void(Byte*)> m_destructor;
		size_t m_currentTypeHash = kInvalidValue<size_t>;

	public:
		constexpr Any() = default;
		constexpr ~Any();

		template<typename T> constexpr Any(T& value)  requires NotSameType<T, Any>;
		template<typename T> constexpr Any(T&& value) requires NotSameType<T, Any>;

		template<typename T> constexpr Any& operator=(T& value)  requires NotSameType<T, Any>;
		template<typename T> constexpr Any& operator=(T&& value) requires NotSameType<T, Any>;

		template<typename T> constexpr       T& As()       requires NotSameType<T, Any>;
		template<typename T> constexpr const T& As() const requires NotSameType<T, Any>;

		template<typename T> constexpr bool Is();

	private:
		template<typename T> constexpr void Construct(T& value)  requires NotSameType<T, Any>;
		template<typename T> constexpr void Construct(T&& value) requires NotSameType<T, Any>;

		constexpr void Destruct();

		template<typename T> constexpr void SetHelpers();
	};

	constexpr Any::~Any()
	{
		Destruct();
	}

	constexpr void Any::Destruct()
	{
		if (m_destructor.IsSet())
		{
			m_destructor(m_pBuffer);
		}
		
		JPT_SAFE_DELETE_ARRAY(m_pBuffer);

		m_destructor.Clear();
		m_currentTypeHash = kInvalidValue<size_t>;
	}

	template<typename T>
	constexpr Any::Any(T& value) requires NotSameType<T, Any>
	{
		Construct(value);
	}

	template<typename T>
	constexpr Any::Any(T&& value) requires NotSameType<T, Any>
	{
		Construct(Move(value));
	}

	template<typename T>
	constexpr Any& Any::operator=(T& value) requires NotSameType<T, Any>
	{
		Destruct();
		Construct(value);
		return *this;
	}

	template<typename T>
	constexpr Any& Any::operator=(T&& value) requires NotSameType<T, Any>
	{
		Destruct();
		Construct(Move(value));
		return *this;
	}

	template<typename T>
	constexpr T& Any::As() requires NotSameType<T, Any>
	{
		JPT_ASSERT(Is<T>(), "Any should be assigned to the given T before calling As()");
		return reinterpret_cast<T&>(*m_pBuffer);
	}

	template<typename T>
	constexpr const T& Any::As() const requires NotSameType<T, Any>
	{
		JPT_ASSERT(Is<T>(), "Any should be assigned to the given T before calling As()");
		return reinterpret_cast<const T&>(*m_pBuffer);
	}

	template<typename T>
	constexpr bool Any::Is()
	{
		return m_currentTypeHash == typeid(T).hash_code();
	}

	template<typename T>
	constexpr void Any::Construct(T& value) requires NotSameType<T, Any>
	{
		m_pBuffer = new Byte[sizeof(T)];
		new (m_pBuffer) T(value);
		SetHelpers<T>();
	}

	template<typename T>
	constexpr void Any::Construct(T&& value) requires NotSameType<T, Any>
	{
		m_pBuffer = new Byte[sizeof(T)];
		new (m_pBuffer) T(Move(value));
		SetHelpers<T>();
	}

	template<typename T>
	constexpr void Any::SetHelpers()
	{
		m_destructor = [](Byte* pBuffer) 
			{
				if constexpr (!IsTriviallyDestructible<T>)
				{
					reinterpret_cast<T*>(pBuffer)->~T();
				}
			};

		m_currentTypeHash = typeid(T).hash_code();
	}
}