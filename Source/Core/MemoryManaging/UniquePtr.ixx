// Copyright Jupiter Technologies, Inc. All Rights Reserved.

module;

#include "Core/Building/Headers.h"

#include <type_traits>

export module jpt.UniquePtr;

import jpt.TypeDefs;
import jpt.Utilities;

namespace jpt
{
	template<class T>
	struct DefaultDelete
	{
		/**  Constructs a jpt::DefaultDelete object. */
		constexpr DefaultDelete() noexcept = default;

		/** Constructs a jpt::DefaultDelete<Type> object from another jpt::DefaultDelete object.
			This constructor will only participate in overload resolution if OtherType* is implicitly convertible to Type* */
		template<class OtherT>
		constexpr DefaultDelete(const DefaultDelete<OtherT>& deleter) noexcept {}

		template<class OtherT>
		constexpr DefaultDelete& operator=(const DefaultDelete<OtherT>&) { return *this; }

		constexpr void operator()(T* pPtr) const { delete pPtr; }
	};

	template<class T>
	struct DefaultDelete<T[]>
	{
		/**  Constructs a jpt::DefaultDelete object. */
		constexpr DefaultDelete() noexcept = default;

		/** Constructs a jpt::DefaultDelete<Type> object from another jpt::DefaultDelete object.
			This constructor will only participate in overload resolution if OtherType* is implicitly convertible to Type* */
		template<class OtherT>
		constexpr DefaultDelete(const DefaultDelete<OtherT[]>& deleter) noexcept {}

		template<class OtherT>
		DefaultDelete& operator=(const DefaultDelete<OtherT[]>&) { return *this; }

		constexpr void operator()(T* pPtr) const { delete[] pPtr; }
	};

	export template<typename T, class Deleter = DefaultDelete<T>>
	class UniquePtr
	{
	private:
		T* m_pPtr = nullptr;	/**< The pointer this UniquePtr is going to guard */
		int32* m_pRefCount = nullptr;

	public:
		constexpr UniquePtr() noexcept = default;
		constexpr UniquePtr(T* pPtr) noexcept;

		constexpr UniquePtr(const UniquePtr&) = delete;
		UniquePtr& operator=(const UniquePtr&) = delete;


		~UniquePtr();
	};

	/** Constructs a non-array type T. The arguments args are passed to the constructor of T. 
		This overload participates in overload resolution only if T is not an array type. 
		The function is equivalent to: UniquePtr<T>(new T(std::forward<Args>(args)...)) */
	export template<typename T, class... Args>
	[[nodiscard]] constexpr UniquePtr<T> MakeUnique(Args&&...args)
	{
		return UniquePtr<T>(new T(jpt::Forward<Args>(args)...));
	}

	template<typename T, class Deleter>
	constexpr UniquePtr<T, Deleter>::UniquePtr(T* pPtr) noexcept
		: m_pPtr(pPtr)
		, m_pRefCount(new int32(1))
	{
	}

	template<typename T, class Deleter>
	UniquePtr<T, Deleter>::~UniquePtr()
	{
		--*m_pRefCount;
		if (*m_pRefCount < 1)
		{
			Deleter deleter;
			deleter(m_pPtr);

			JPT_DELETE(m_pRefCount);
		}
	}
}