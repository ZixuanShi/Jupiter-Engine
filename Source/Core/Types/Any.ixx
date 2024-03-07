// Copyright Jupiter Technologies, Inc. All Rights Reserved.

module;

#include "Core/Minimal/Macros.h"
#include "Debugging/Assert.h"

#include <typeinfo>
#include <string>

export module jpt.Any;

import jpt.Allocator;
import jpt.Byte;
import jpt.Constants;
import jpt.TypeTraits;
import jpt.Utilities;

namespace jpt
{
	/** Can hold any type 
		@note	When copying or moving another Any object. Do not use the default member copy/move constructor/operator=. Instead use CopyAny or MoveAny
		@example 
			
			// Assigning and retrieving values
			jpt::Any any;
			any = 42;
			JPT_LOG(any.Is<int>()); // true
			JPT_LOG(any.As<int>()); // 42
			any = jpt::String("Hello, World!");
			JPT_LOG(any.Is<jpt::String>()); // true
			JPT_LOG(any.As<jpt::String>()); // Hello, World!

			// DON'T DO THIS
			jpt::Any any2;
			any2 = any; // This will cause a runtime infinite constructing loop and heap will overflow

			// DO THIS
			any2.CopyAny<jpt::String>(any);
			any2.MoveAny(Move(any));       		*/
	export class Any
	{
		using Destructor = void(*)(Byte*);

	private:
		Byte* m_pBuffer = nullptr;          /**< Dynamically resizing buffer that will hold any data when assigning & constructing */
		Destructor m_destructor = nullptr;	/**< Function pointer to the destructor of the current type */
		size_t m_currentTypeHash = 0;     /**< Hash code of the current type. Used for comparing */

	public:
		constexpr Any() = default;
		constexpr ~Any();

		template<typename T> constexpr Any(T& value);
		template<typename T> constexpr Any(T&& value);

		template<typename T> constexpr Any& operator=(T& value);
		template<typename T> constexpr Any& operator=(T&& value);

		template<typename T> constexpr       T& As();
		template<typename T> constexpr const T& As() const;

		template<typename T> constexpr bool Is() const;

		template<typename T> constexpr void CopyAny(Any& other);
		constexpr void MoveAny(Any&& other);

	private:
		template<typename T> constexpr void Construct(T& value);
		template<typename T> constexpr void Construct(T&& value);

		/** Adapt to new Type, the new type is guaranteed not the same as current type */
		template<typename T> constexpr void Adapt();

		constexpr void Destruct();
	};

	constexpr Any::~Any()
	{
		Destruct();
		JPT_SAFE_DELETE_ARRAY(m_pBuffer);
		m_destructor = nullptr;
		m_currentTypeHash = 0;
	}

	constexpr void Any::Destruct()
	{
		if (m_destructor)
		{
			m_destructor(m_pBuffer);
		}
	}

	template<typename T>
	constexpr Any::Any(T& value)
	{
		Construct(value);
	}

	template<typename T>
	constexpr Any::Any(T&& value)
	{
		Construct(Move(value));
	}

	template<typename T>
	constexpr Any& Any::operator=(T& value)
	{
		if (Is<T>())
		{
			As<T>() = value;
			return *this;
		}

		Destruct();
		Construct(value);
		return *this;
	}

	template<typename T>
	constexpr Any& Any::operator=(T&& value)
	{
		if (Is<T>())
		{
			As<T>() = Move(value);
			return *this;
		}

		Destruct();
		Construct(Move(value));
		return *this;
	}

	template<typename T>
	constexpr T& Any::As()
	{
		JPT_ASSERT(Is<T>(), "Any should be assigned to the given T before calling As()");
		return reinterpret_cast<T&>(*m_pBuffer);
	}

	template<typename T>
	constexpr const T& Any::As() const
	{
		JPT_ASSERT(Is<T>(), "Any should be assigned to the given T before calling As()");
		return reinterpret_cast<const T&>(*m_pBuffer);
	}

	template<typename T>
	constexpr bool Any::Is() const
	{
		return m_currentTypeHash == typeid(T).hash_code();
	}

	template<typename T>
	constexpr void Any::CopyAny(Any& other)
	{
		Construct(other.As<T>());
	}

	constexpr void Any::MoveAny(Any&& other)
	{
		m_pBuffer = other.m_pBuffer;
		m_destructor = Move(other.m_destructor);
		m_currentTypeHash = other.m_currentTypeHash;

		other.m_pBuffer = nullptr;
		other.m_destructor = nullptr;
		other.m_currentTypeHash = 0;
	}

	template<typename T>
	constexpr void Any::Construct(T& value)
	{
		Adapt<T>();
		Allocator<T>::Construct(reinterpret_cast<T*>(m_pBuffer), value);
	}

	template<typename T>
	constexpr void Any::Construct(T&& value)
	{
		Adapt<T>();
		Allocator<T>::Construct(reinterpret_cast<T*>(m_pBuffer), Move(value));
	}

	template<typename T>
	constexpr void Any::Adapt()
	{
		JPT_DELETE_ARRAY(m_pBuffer);
		m_pBuffer = new Byte[sizeof(T)];

		// Assign destructor function to current T
		m_destructor = [](Byte* pBuffer)
			{
				if constexpr (!IsTriviallyDestructible<T>)
				{
					reinterpret_cast<T*>(pBuffer)->~T();
				}
			};

		// Current type hash is set to current T
		m_currentTypeHash = typeid(T).hash_code();
	}
}