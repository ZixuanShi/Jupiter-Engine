// Copyright Jupiter Technologies, Inc. All Rights Reserved.

module;

#include "Core/Building/Macros.h"
#include "Core/Debugging/Assert.h"

export module jpt.String;

import jpt.Allocator;
import jpt.TypeDefs;
import jpt.StringUtils;

namespace jpt
{
	/** Resizing multiplier for capacity */
	static constexpr size_t kCapacityMultiplier = 2;

	template<typename _CharType, class _AllocatorType = Allocator<_CharType>>
	class BasicString
	{
	public:
		using CharType	    = _CharType;
		using AllocatorType = _AllocatorType;

	public:
		static constexpr size_t npos = kInvalidValue<size_t>;

	private:
		CharType* m_pBuffer = nullptr;	/**< The pointer to the buffer representing this string's value */
		size_t m_size       = 0;		/**< How many characters in this string currently */
		size_t m_capacity   = 0;		/**< How many characters this string can hold before resizing */

	public:
		BasicString() = default;
		BasicString(const CharType* CString);
		BasicString(const BasicString<CharType>& otherString);
		~BasicString();

		// Element Access
		const CharType* ConstBuffer() const { return m_pBuffer; }
		CharType* Buffer() const { return m_pBuffer; }
		CharType& operator[](size_t index) { return m_pBuffer[index]; }
		const CharType& operator[](size_t index) const { return m_pBuffer[index]; }
		CharType& Back() { return m_pBuffer[m_size - 1]; }
		const CharType& Back() const { return m_pBuffer[m_size - 1]; }
		CharType& Front() { return m_pBuffer[0]; }
		const CharType& Front() const { return m_pBuffer[0]; }

		// Capacity
		size_t size() const { return m_size; }
		size_t capacity() const { return m_capacity; }
		bool empty() const { return m_size == 0; }

		/* Deallocate the memory that this string holds */
		void Destroy();

		/* Copy the content of string. Will assign the current m_pBuffer with the new copied data in memory */
		void CopyCString(const CharType* inCString);
		void CopyString(const BasicString<CharType>& otherString);

	private:
		/* Called when the current buffer is not big enough to hold a new string to append. Update the buffer to a larger sizeand increase capacity
			@param inCapacity: The capacity for the new buffer. Typically current m_size * kCapacityMultiplier */
		void UpdateBuffer(size_t inCapacity);
	};

	template<typename _CharType, class _AllocatorType>
	BasicString<_CharType, _AllocatorType>::BasicString(const CharType* CString)
	{
		CopyCString(CString);
	}

	template<typename _CharType, class _AllocatorType>
	BasicString<_CharType, _AllocatorType>::BasicString(const BasicString<CharType>& otherString)
	{
		CopyString(otherString);
	}

	template<typename _CharType, class _AllocatorType>
	BasicString<_CharType, _AllocatorType>::~BasicString()
	{
		Destroy();
	}

	template<typename _CharType, class _AllocatorType>
	void BasicString<_CharType, _AllocatorType>::Destroy()
	{
		JPT_SAFE_DELETE_ARRAY(m_pBuffer);
		m_size = 0;
		m_capacity = 0;
	}

	template<typename _CharType, class _AllocatorType>
	void BasicString<_CharType, _AllocatorType>::CopyCString(const CharType* inCString)
	{
		m_size = GetStrLength(inCString);
		if (empty())
		{
			return;
		}

		UpdateBuffer(m_size * kCapacityMultiplier);		// Reserve some memory storage to append stuff
		JPT_ASSERT(m_pBuffer, "m_pBuffer shouldn't be nullptr");
		StrCpy(m_pBuffer, m_size + sizeof(CharType), inCString);
	}

	template<typename _CharType, class _AllocatorType>
	void BasicString<_CharType, _AllocatorType>::CopyString(const BasicString<CharType>& otherString)
	{
		m_size = otherString.size();
		if (empty())
		{
			return;
		}

		UpdateBuffer(m_size * kCapacityMultiplier);		// Reserve some memory storage to append stuff
		JPT_ASSERT(m_pBuffer, "m_pBuffer shouldn't be nullptr");
		StrCpy(m_pBuffer, m_size + sizeof(CharType), otherString.ConstBuffer());
	}

	template<typename _CharType, class _AllocatorType>
	void BasicString<_CharType, _AllocatorType>::UpdateBuffer(size_t inCapacity)
	{
		CharType* pNewBuffer = new CharType[inCapacity + sizeof(CharType)];

		if (m_pBuffer)
		{
			StrCpy(pNewBuffer, m_size + sizeof(CharType), m_pBuffer);
			delete[] m_pBuffer;
		}

		m_pBuffer = pNewBuffer;
		m_capacity = inCapacity;
	}
}

export namespace jpt
{
	using String = BasicString<char>;
	using WString = BasicString<wchar_t>;
}