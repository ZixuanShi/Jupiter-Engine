// Copyright Jupiter Technologies, Inc. All Rights Reserved.

module;

#include "Debugging/Assert.h"
#include <type_traits>

export module jpt.Optional;

import jpt.Utilities;

export namespace jpt
{
	/** Represents an optionally contained value. The value will only be valid when has_value() returns true
		This is useful in many scenarios, such as return a optional result from function that may fail. */
	template<typename _TValue>
	class Optional
	{
	public:
		using TValue = _TValue;

	private:
		TValue m_value;
		bool m_hasValue : 1 = false;

	public:
		constexpr Optional() = default;
		constexpr Optional(const TValue& value);
		constexpr Optional(TValue&& value);
		constexpr Optional(const Optional& other);
		constexpr Optional(Optional&& other) noexcept;
		Optional& operator=(const TValue& value);
		Optional& operator=(TValue&& value);
		Optional& operator=(const Optional& other);
		Optional& operator=(Optional&& other) noexcept;
		constexpr ~Optional();

		/** Destroys any contained value */
		constexpr void Reset();

		/** @return true if this Has a value. */
		constexpr bool HasValue() const { return m_hasValue; }
		constexpr operator bool() const { return m_hasValue; }

		constexpr const TValue& Value()  const { JPT_ASSERT(m_hasValue, "Accessing optional data when it's not set"); return m_value;  }
		constexpr       TValue& Value()        { JPT_ASSERT(m_hasValue, "Accessing optional data when it's not set"); return m_value;  }
		constexpr const TValue* GetPtr() const { JPT_ASSERT(m_hasValue, "Accessing optional data when it's not set"); return &m_value; }
		constexpr       TValue* GetPtr()       { JPT_ASSERT(m_hasValue, "Accessing optional data when it's not set"); return &m_value; }

	private:
		constexpr void CopyData(const Optional& other);
		constexpr void MoveData(Optional&& other);
	};

	template<typename TValue>
	constexpr Optional<TValue>::Optional(const TValue& value)
		: m_value(value)
		, m_hasValue(true)
	{
	}

	template<typename TValue>
	constexpr Optional<TValue>::Optional(TValue&& value)
		: m_value(Move(value))
		, m_hasValue(true)
	{
	}

	template<typename TValue>
	constexpr Optional<TValue>::Optional(const Optional& other)
	{
		CopyData(other);
	}

	template<typename TValue>
	constexpr Optional<TValue>::Optional(Optional&& other) noexcept
	{
		MoveData(Move(other));
	}

	template<typename TValue>
	Optional<TValue>& Optional<TValue>::operator=(const TValue& value)
	{
		Reset();
		m_value = value;
		m_hasValue = true;
		return *this;
	}

	template<typename TValue>
	Optional<TValue>& Optional<TValue>::operator=(TValue&& value)
	{
		Reset();
		m_value = Move(value);
		m_hasValue = true;
		return *this;
	}

	template<typename TValue>
	Optional<TValue>& Optional<TValue>::operator=(const Optional& other)
	{
		if (this != &other)
		{
			Reset();
			CopyData(other);
		}

		return *this;
	}

	template<typename TValue>
	Optional<TValue>& Optional<TValue>::operator=(Optional&& other) noexcept
	{
		if (this != &other)
		{
			Reset();
			MoveData(Move(other));
		}

		return *this;
	}

	template<typename TValue>
	constexpr Optional<TValue>::~Optional()
	{
		Reset();
	}

	template<typename TValue>
	constexpr void Optional<TValue>::Reset()
	{
		if (!m_hasValue)
		{
			return;
		}

		if constexpr (!std::is_trivially_default_constructible_v<TValue>)
		{
			m_value.~TValue();
		}

		m_hasValue = false;
	}

	template<typename TValue>
	constexpr void Optional<TValue>::CopyData(const Optional& other)
	{
		m_value = other.m_value;
		m_hasValue = other.m_hasValue;
	}

	template<typename TValue>
	constexpr void Optional<TValue>::MoveData(Optional&& other)
	{
		m_value = Move(other.m_value);
		m_hasValue = other.m_hasValue;
		other.Reset();
	}
}
