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
			JPT_LOG(any.As<jpt::String>()); // Hello, World! */
	export class Any
	{
		using Destructor = void(*)(Byte*);

	private:
		Byte* m_pBuffer = nullptr;          /**< Dynamically resizing buffer that will hold any data when assigning & constructing */
		Destructor m_destructor = nullptr;	/**< Function pointer to the destructor of the current type */
		size_t m_currentTypeHash = 0;       /**< Hash code of the current type. Used for comparing */

	public:
		constexpr Any() = default;
		constexpr ~Any();
		//constexpr Any(const Any& other);
		//constexpr Any(Any&& other) noexcept;
		//constexpr Any& operator=(const Any& other);
		//constexpr Any& operator=(Any&& other) noexcept;

		template<typename T> 
		requires NotSameType<T, Any>
		constexpr Any(const T& value);

		template<typename T> 
		requires IsSameType<T, typename RemoveReference<T>::Type> && NotSameType<T, Any>
		constexpr Any(T&& value);

		template<typename T> 
		requires NotSameType<T, Any>
		constexpr Any& operator=(const T& value);

		template<typename T> 
		requires IsSameType<T, typename RemoveReference<T>::Type> && NotSameType<T, Any>
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
		constexpr void Construct(const T& value);

		template<typename T> 
		requires IsSameType<T, typename RemoveReference<T>::Type> && NotSameType<T, Any>
		constexpr void Construct(T&& value);

		/** Adapt to new Type, the new type is guaranteed not the same as current type */
		template<typename T> 
		constexpr void Adapt();

		constexpr void Destruct();
	};

	constexpr Any::~Any()
	{
		Destruct();
		Allocator<Byte>::DeallocateArray(m_pBuffer);
		m_pBuffer = nullptr;
		m_destructor = nullptr;
		m_currentTypeHash = 0;
	}

	//constexpr Any::Any(const Any& other)
	//{
	//}

	//constexpr Any::Any(Any&& other) noexcept
	//{
	//}

	//constexpr Any& Any::operator=(const Any& other)
	//{
	//	// TODO: insert return statement here
	//}

	//constexpr Any& Any::operator=(Any&& other) noexcept
	//{
	//	// TODO: insert return statement here
	//}

	constexpr void Any::Destruct()
	{
		if (m_destructor)
		{
			m_destructor(m_pBuffer);
		}
	}

	template<typename T>
	requires NotSameType<T, Any>
	constexpr Any::Any(const T& value)
	{
		Construct(value);
	}

	template<typename T>
	requires IsSameType<T, typename RemoveReference<T>::Type> && NotSameType<T, Any>
	constexpr Any::Any(T&& value)
	{
		Construct(Move(value));
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

		Destruct();
		Construct(value);
		return *this;
	}

	template<typename T>
	requires IsSameType<T, typename RemoveReference<T>::Type> && NotSameType<T, Any>
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
	requires NotSameType<T, Any>
	constexpr void Any::Construct(const T& value)
	{
		Adapt<T>();
		Allocator<T>::Construct(reinterpret_cast<T*>(m_pBuffer), value);
	}

	template<typename T>
	requires IsSameType<T, typename RemoveReference<T>::Type> && NotSameType<T, Any>
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