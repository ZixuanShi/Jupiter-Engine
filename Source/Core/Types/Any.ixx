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

static constexpr size_t kLocSmallDataSize = 8;

export namespace jpt
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
			JPT_LOG(any.As<jpt::String>()); // Hello, World! */
	class Any
	{
		using Destructor = void(*)(Byte*);

	private:
		Byte m_smallBuffer[kLocSmallDataSize]; /**< Small Object Optimization. Will hold data in this local stack buffer if it's small */
		Byte* m_pBuffer = nullptr;          /**< Buffer pointer to the memory of hold object */
		Destructor m_destructor = nullptr;	/**< Function pointer to the destructor of the current type */
		size_t m_currentTypeHash = 0;       /**< Hash code of the current type. Used for comparing */
		size_t m_currentTypeSize = 0;       /**< Size of the current type */

	public:
		constexpr Any() = default;
		constexpr ~Any();

		template<typename T> 
		requires NotSameType<T, Any>
		constexpr Any(const T& value);

		template<typename T> 
		requires NotSameType<T, Any> && IsSameType<T, TRemoveReference<T>>
		constexpr Any(T&& value);

		template<typename T> 
		requires NotSameType<T, Any>
		constexpr Any& operator=(const T& value);

		template<typename T> 
		requires NotSameType<T, Any> && IsSameType<T, TRemoveReference<T>>
		constexpr Any& operator=(T&& value);

		template<typename T> 
		constexpr T& As();

		template<typename T> 
		constexpr const T& As() const;

		template<typename T> 
		constexpr bool Is() const;

	private:
		template<typename T> 
		requires NotSameType<T, Any>
		constexpr void ConstructType(const T& value);

		template<typename T> 
		requires NotSameType<T, Any> && IsSameType<T, TRemoveReference<T>>
		constexpr void ConstructType(T&& value);

		/** Adapt to new Type, the new type is guaranteed not the same as current type */
		template<typename T> 
		constexpr void Adapt();

		constexpr void DestructObject();
		constexpr void DeallocateBuffer();
	};

	constexpr Any::~Any()
	{
		DestructObject();
		DeallocateBuffer();
		m_pBuffer = nullptr;
		m_destructor = nullptr;
		m_currentTypeHash = 0;
	}

	constexpr void Any::DestructObject()
	{
		if (m_destructor)
		{
			m_destructor(m_pBuffer);
		}
	}

	constexpr void Any::DeallocateBuffer()
	{
		if (m_pBuffer &&
			m_pBuffer != m_smallBuffer)
		{
			Allocator<Byte>::DeallocateArray(m_pBuffer);
		}
	}

	template<typename T>
	requires NotSameType<T, Any>
	constexpr Any::Any(const T& value)
	{
		ConstructType(value);
	}

	template<typename T>
	requires NotSameType<T, Any> && IsSameType<T, TRemoveReference<T>>
	constexpr Any::Any(T&& value)
	{
		ConstructType(Move(value));
	}

	template<typename T>
	requires NotSameType<T, Any>
	constexpr Any& Any::operator=(const T& value) 
	{
		if (Is<T>())
		{
			As<T>() = value;
			return *this;
		}

		DestructObject();
		ConstructType(value);
		return *this;
	}

	template<typename T>
	requires NotSameType<T, Any> && IsSameType<T, TRemoveReference<T>>
	constexpr Any& Any::operator=(T&& value)
	{
		if (Is<T>())
		{
			As<T>() = Move(value);
			return *this;
		}

		DestructObject();
		ConstructType(Move(value));
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
		return m_currentTypeSize == sizeof(T) && m_currentTypeHash == typeid(T).hash_code();
	}

	template<typename T>
	requires NotSameType<T, Any>
	constexpr void Any::ConstructType(const T& value)
	{
		Adapt<T>();
		Allocator<T>::Construct(reinterpret_cast<T*>(m_pBuffer), value);
	}

	template<typename T>
	requires NotSameType<T, Any> && IsSameType<T, TRemoveReference<T>>
	constexpr void Any::ConstructType(T&& value)
	{
		Adapt<T>();
		Allocator<T>::Construct(reinterpret_cast<T*>(m_pBuffer), Move(value));
	}

	template<typename T>
	constexpr void Any::Adapt()
	{
		// At this point, the current type is guaranteed not the same as T
		// But size might be the same. If it's not, we need to reallocate the buffer
		const size_t newSize = sizeof(T);

		if (newSize != m_currentTypeSize)
		{
			DeallocateBuffer();
		}

		if constexpr (newSize <= kLocSmallDataSize)
		{
			m_pBuffer = m_smallBuffer;
		}
		else
		{
			m_pBuffer = Allocator<Byte>::AllocateArray(newSize);
		}

		// Assign destructor function to current T
		m_destructor = [](Byte* pBuffer)
			{
				if constexpr (!IsTriviallyDestructible<T>)
				{
					reinterpret_cast<T*>(pBuffer)->~T();
				}
			};

		// Update holding data type info
		m_currentTypeHash = typeid(T).hash_code();
		m_currentTypeSize = newSize;
	}
}