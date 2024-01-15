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
	template<typename _ValueT>
	class Optional
	{
	public:
		using ValueT = _ValueT;

	private:
		ValueT m_value;
		bool m_hasValue = false;

	public:
		Optional() = default;
		Optional(const ValueT& value);
		Optional(ValueT&& value);
		Optional(const Optional& other);
		Optional(Optional&& other) noexcept;
		Optional& operator=(const ValueT& value);
		Optional& operator=(ValueT&& value);
		Optional& operator=(const Optional& other);
		Optional& operator=(Optional&& other) noexcept;
		~Optional();

		/** Destroys any contained value */
		void Reset();

		/** @return true if this contains a value. */
		bool HasValue() const { return m_hasValue; }
		operator bool() const { return m_hasValue; }

		const ValueT& Value() const { JPT_ASSERT(m_hasValue); return m_value; }
		ValueT& Value() { JPT_ASSERT(m_hasValue); return m_value; }
		const ValueT* GetPtr() const { JPT_ASSERT(m_hasValue); return &m_value; }
		ValueT* GetPtr() { JPT_ASSERT(m_hasValue); return &m_value; }

	private:
		void CopyData(const Optional& other);
		void MoveData(Optional&& other);
	};

	template<typename ValueT>
	inline Optional<ValueT>::Optional(const ValueT& value)
		: m_value(value)
		, m_hasValue(true)
	{
	}

	template<typename ValueT>
	inline Optional<ValueT>::Optional(ValueT&& value)
		: m_value(Move(value))
		, m_hasValue(true)
	{
	}

	template<typename ValueT>
	inline Optional<ValueT>::Optional(const Optional& other)
	{
		CopyData(other);
	}

	template<typename ValueT>
	inline Optional<ValueT>::Optional(Optional&& other) noexcept
	{
		MoveData(Move(other));
	}

	template<typename ValueT>
	inline Optional<ValueT>& Optional<ValueT>::operator=(const ValueT& value)
	{
		Reset();
		m_value = value;
		m_hasValue = true;
		return *this;
	}

	template<typename ValueT>
	inline Optional<ValueT>& Optional<ValueT>::operator=(ValueT&& value)
	{
		Reset();
		m_value = Move(value);
		m_hasValue = true;
		return *this;
	}

	template<typename ValueT>
	inline Optional<ValueT>& Optional<ValueT>::operator=(const Optional& other)
	{
		if (this != &other)
		{
			Reset();
			CopyData(other);
		}

		return *this;
	}

	template<typename ValueT>
	inline Optional<ValueT>& Optional<ValueT>::operator=(Optional&& other) noexcept
	{
		if (this != &other)
		{
			Reset();
			MoveData(Move(other));
		}

		return *this;
	}

	template<typename ValueT>
	inline Optional<ValueT>::~Optional()
	{
		Reset();
	}

	template<typename ValueT>
	void Optional<ValueT>::Reset()
	{
		if (!m_hasValue)
		{
			return;
		}

		if constexpr (!std::is_trivially_default_constructible_v<ValueT>)
		{
			m_value.~ValueT();
		}

		m_hasValue = false;
	}

	template<typename ValueT>
	inline void Optional<ValueT>::CopyData(const Optional& other)
	{
		m_value = other.m_value;
		m_hasValue = other.m_hasValue;
	}

	template<typename ValueT>
	inline void Optional<ValueT>::MoveData(Optional&& other)
	{
		m_value = move(other.m_value);
		m_hasValue = other.m_hasValue;
		other.Reset();
	}
}
