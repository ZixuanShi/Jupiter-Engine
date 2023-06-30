// Copyright Jupiter Technologies, Inc. All Rights Reserved.

#pragma once

namespace jpt
{
	/** Represents an optionally contained value. The value will only be valid when has_value() returns true 
		This is useful in many scenarios, such as return a optional result from function that may fail. */
	template<typename _ValueType>
	class optional
	{
	public:
		using ValueType = _ValueType;

	private:
		ValueType m_value;
		bool m_hasValue = false;

	public:
		optional() = default;
		optional(const ValueType& value);
		optional(ValueType&& value);
		optional(const optional& other);
		optional(optional&& other) noexcept;
		optional& operator=(const ValueType& value);
		optional& operator=(ValueType&& value);
		optional& operator=(const optional& other);
		optional& operator=(optional&& other) noexcept;
		~optional();

		/** Destroys any contained value */
		void reset();

		/** @return true if this contains a value. */
		bool has_value() const { return m_hasValue; }

		const ValueType& value() const { JPT_ASSERT(m_hasValue); return m_value; }
		ValueType& value() { JPT_ASSERT(m_hasValue); return m_value; }

	private:
		void Copy(const optional& other);
		void Move(optional&& other);
	};

	template<typename ValueType>
	inline optional<ValueType>::optional(const ValueType& value)
		: m_value(value)
		, m_hasValue(true)
	{
	}

	template<typename ValueType>
	inline optional<ValueType>::optional(ValueType&& value)
		: m_value(jpt::move(value))
		, m_hasValue(true)
	{
	}

	template<typename ValueType>
	inline jpt::optional<ValueType>::optional(const optional& other)
	{
		Copy(other);
	}

	template<typename ValueType>
	inline jpt::optional<ValueType>::optional(optional&& other) noexcept
	{
		Move(jpt::move(other));
	}

	template<typename ValueType>
	inline optional<ValueType>& jpt::optional<ValueType>::operator=(const ValueType& value)
	{
		reset();
		m_value = value;
		m_hasValue = true;
		return *this;
	}

	template<typename ValueType>
	inline optional<ValueType>& jpt::optional<ValueType>::operator=(ValueType&& value)
	{
		reset();
		m_value = jpt::move(value);
		m_hasValue = true;
		return *this;
	}

	template<typename ValueType>
	inline optional<ValueType>& jpt::optional<ValueType>::operator=(const optional& other)
	{
		if (this == &other)
		{
			return *this;
		}

		reset();
		Copy(other);
		return *this;
	}

	template<typename ValueType>
	inline optional<ValueType>& jpt::optional<ValueType>::operator=(optional&& other) noexcept
	{
		if (this == &other)
		{
			return *this;
		}

		reset();
		Move(jpt::move(other));
		return *this;
	}

	template<typename ValueType>
	inline jpt::optional<ValueType>::~optional()
	{
		reset();
	}

	template<typename ValueType>
	void jpt::optional<ValueType>::reset()
	{
		if (!m_hasValue)
		{
			return;
		}

		if constexpr (!std::is_trivially_default_constructible_v<ValueType>)
		{
			m_value.~ValueType();
		}

		m_hasValue = false;
	}
	
	template<typename _ValueType>
	inline void jpt::optional<_ValueType>::Copy(const optional& other)
	{
		m_value = other.m_value;
		m_hasValue = other.m_hasValue;
	}

	template<typename _ValueType>
	inline void jpt::optional<_ValueType>::Move(optional&& other)
	{
		m_value = jpt::move(other.m_value);
		m_hasValue = other.m_hasValue;
		other.reset();
	}
}