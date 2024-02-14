// Copyright Jupiter Technologies, Inc. All Rights Reserved.

module;

#include "Core/Minimal/Macros.h"
#include "Debugging/Assert.h"

#include <typeinfo>
#include <typeindex>

export module jpt.Any;

import jpt.Byte;
import jpt.Constants;
import jpt.TypeTraits;

export namespace jpt
{
	class Any
	{
	private:
		Byte* m_pBuffer = nullptr;
		size_t m_currentTypeHashCode = kInvalidValue<size_t>;

	public:
		constexpr Any() = default;
		constexpr ~Any();

		template<typename T>
		constexpr Any(const T& value);

		template<typename T>
		constexpr Any& operator=(const T& value);

		template<typename T> constexpr       T& As();
		template<typename T> constexpr const T& As() const;

	private:
		template<typename T>
		constexpr void Construct(const T& value);

		constexpr void Destruct();
	};

	constexpr Any::~Any()
	{
		Destruct();
	}

	constexpr void Any::Destruct()
	{
		// TODO: Try to find a way to call current type's destructor if it's not trivially destructible
		//if constexpr (!IsTriviallyDestructible<TCurrent>)
		//{
		//	reinterpret_cast<TCurrent*>(m_buffer)->~TCurrent();
		//}

		JPT_SAFE_DELETE_ARRAY(m_pBuffer);
	}

	template<typename T>
	constexpr Any::Any(const T& value)
	{
		Construct(value);
	}

	template<typename T>
	constexpr Any& Any::operator=(const T& value)
	{
		Destruct();
		Construct(value);
		return *this;
	}

	template<typename T>
	constexpr T& Any::As()
	{
		return reinterpret_cast<T&>(*m_pBuffer);
	}

	template<typename T>
	constexpr const T& Any::As() const
	{
		return reinterpret_cast<const T&>(*m_pBuffer);
	}

	template<typename T>
	constexpr void Any::Construct(const T& value)
	{
		m_pBuffer = new Byte[sizeof(T)];
		reinterpret_cast<T&>(*m_pBuffer) = value;
	}
}