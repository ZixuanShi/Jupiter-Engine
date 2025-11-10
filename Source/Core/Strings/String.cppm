// Copyright Jupiter Technologies, Inc. All Rights Reserved.

module;

#include "Core/Strings/StringMacros.h"
#include "Core/Memory/Memory.h"
#include "Core/Minimal/Utilities.h"
#include "Core/Validation/Assert.h"

#include <stdio.h>
#include <vadefs.h>
#include <stdarg.h>
#include <fstream>

export module jpt.String;

import jpt.Allocator;
import jpt.Concepts;
import jpt.Constants;
import jpt.DynamicArray;
import jpt.Hash;
import jpt.Math;
import jpt.StringHelpers;
import jpt.TypeDefs;
import jpt.Utilities;
import jpt.Serializer;

export namespace jpt
{
    template<StringLiteral _TChar, class _TAllocator = Allocator<_TChar>>
    class String_Base
    {
    public:
        using TChar         = _TChar;
        using TData         = _TChar;
        using TAllocator    = _TAllocator;
        using Iterator      = TChar*;
        using ConstIterator = const TChar*;

    private:
        TChar m_smallBuffer[kSmallDataSize] = { 0 };    /**< Small buffer to store small data */
        TChar* m_pBuffer = nullptr;  /**< The pointer to the buffer representing this string's value */
        Index m_count = 0;          /**< How many characters in this string currently */
        Index m_capacity = 0;       /**< How many characters this string can hold before resizing. Excluded the null terminator */

    public:
        constexpr String_Base() = default;
        constexpr String_Base(const TChar* CString, Index size);
        constexpr String_Base(const TChar* CString);
        constexpr String_Base(TChar c);
        constexpr String_Base(const String_Base<TChar>& otherString);
        constexpr String_Base(String_Base<TChar>&& otherString) noexcept;

        String_Base& operator=(const TChar* CString);
        String_Base& operator=(const String_Base<TChar>& otherString);
        String_Base& operator=(String_Base<TChar>&& otherString) noexcept;
        constexpr ~String_Base();

        // Element Access
        constexpr const TChar* ConstBuffer() const noexcept;
        constexpr       TChar* Buffer()      const noexcept;
        constexpr       TChar& Front()             noexcept;
        constexpr const TChar& Front()       const noexcept;
        constexpr       TChar& Back()              noexcept;
        constexpr const TChar& Back()        const noexcept;
        constexpr       TChar& operator[](Index index)       noexcept;
        constexpr const TChar& operator[](Index index) const noexcept;

        // Iterators
        constexpr Iterator begin() noexcept;
        constexpr Iterator end() noexcept;
        constexpr ConstIterator begin()  const noexcept;
        constexpr ConstIterator cbegin() const noexcept;
        constexpr ConstIterator end()    const noexcept;
        constexpr ConstIterator cend()   const noexcept;

        // Capacity
        constexpr Index Size()     const noexcept; /**< How many size in bytes this string in memory */
        constexpr Index Capacity() const noexcept;
        constexpr bool   IsEmpty()  const noexcept;
        constexpr Index Count()    const noexcept; /**< How many characters in this string */
        constexpr Index Count(TChar c, Index startIndex = 0, Index endIndex = kInvalidIndex) const noexcept; /**< How many times the character c appears in this string */
        constexpr Index Count(const TChar* pString, Index startIndex = 0, Index endIndex = kInvalidIndex) const noexcept;

        /** Searching. Returns kInvalidIndex if not found */
        constexpr Index Find(      TChar  charToFind,    Index startIndex = 0, Index endIndex = kInvalidIndex) const noexcept;
        constexpr Index Find(const TChar* pStringToFind, Index startIndex = 0, Index endIndex = kInvalidIndex) const noexcept;
        constexpr Index FindFirstOf(      TChar  charToFind,    Index startIndex = 0, Index endIndex = kInvalidIndex) const noexcept;
        constexpr Index FindFirstOf(const TChar* pStringToFind, Index startIndex = 0, Index endIndex = kInvalidIndex) const noexcept;
        constexpr Index FindLastOf(      TChar  charToFind,     Index startIndex = 0, Index endIndex = kInvalidIndex)  const noexcept;
        constexpr Index FindLastOf(const TChar* pStringToFind,  Index startIndex = 0, Index endIndex = kInvalidIndex)  const noexcept;
        constexpr bool   Has(      TChar  charToFind,    Index startIndex = 0, Index endIndex = kInvalidIndex) const noexcept;
        constexpr bool   Has(const TChar* pStringToFind, Index startIndex = 0, Index endIndex = kInvalidIndex) const noexcept;
        constexpr bool BeginsWith(const TChar* pStringToFind) const noexcept;
        constexpr bool EndsWith(const TChar* pStringToFind) const noexcept;

        /* Deallocate the memory that this string holds */
        constexpr void Clear();

        /* Replace some content of this string with the given one within optional range
            @param StringToFind:    The string we want to remove and replace
            @param StringToReplace:    The string to replace the original data
            @param startIndex:        [optional] The start index to start searching. Default to 0
            @param endIndex:        [optional] The end index to stop operation. Default to Count() */
        constexpr String_Base& Replace(const TChar* pStringToFind, const TChar* pStringToReplace, Index startIndex = 0, Index endIndex = kInvalidIndex);

        /** Splits this string to substrings by a keyword */
        constexpr DynamicArray<String_Base> Split(const TChar* pKeyword) const;
        constexpr DynamicArray<String_Base> Split(TChar keyword) const;

        /** @return        A sub string within the given range at index and length */
        constexpr String_Base SubStr(Index index, Index count = kInvalidIndex) const;
        constexpr void PopBack(Index count = 1);

        /** Trim string from the left or right at given index
            @param index:    [optional] The index to trim to. Default to kInvalidIndex if just trim out white spaces */
        constexpr void TrimLeft(Index index = kInvalidIndex);
        constexpr void TrimRight(Index index = kInvalidIndex);

        /** Insert a string at the index
            @param CString:      String to insert
            @param index:        Where to insert in this string
            @param size:        [optional] Length of the string to insert */
        constexpr void Insert(TChar c, Index index);
        constexpr void Insert(const TChar* CString, Index index);
        constexpr void Insert(const TChar* CString, Index index, Index size);

        /** Appends a string to the end of buffer */
        constexpr void Append(const TChar* CString, Index newStringSize);
        constexpr void Append(const TChar* CString);
        constexpr void Append(const String_Base<TChar>& otherString);
        constexpr void Append(const DynamicArray<String_Base<TChar>>& strings, const TChar* separator = nullptr);
        constexpr void Append(TChar c);
        constexpr String_Base& operator+=(const TChar* CString);
        constexpr String_Base& operator+=(const String_Base<TChar>& otherString);
        constexpr String_Base& operator+=(TChar c);

        /** Pre allocate buffer with capacity's size. Preventing oftenly dynamic heap allocation */
        constexpr void Reserve(Index capacity);
        constexpr void Resize(Index newSize);

        /** Formats data to a string with provided format then return it
            @example String::Format<32>("%d/%d/%d. %d:%d:%d", month, day, year, hour, minute, second); */
        template<Index kCount>
        static constexpr String_Base Format(const TChar* format, ...);

        /* Copy the content of string. Will assign the current m_pBuffer with the copied data in memory */
        constexpr void CopyString(const TChar* inCString, Index size);
        constexpr void CopyString(const TChar* inCString);
        constexpr void CopyString(const String_Base<TChar>& otherString);

        /* Move the content of string. Will take ownership of the passed in string */
        constexpr void MoveString(TChar* inCString, Index size);
        constexpr void MoveString(TChar* inCString);
        constexpr void MoveString(String_Base<TChar>&& otherString);

        void Serialize(Serializer& serializer) const;
        void Deserialize(Serializer& serializer);

    private:
        constexpr void DeallocateBuffer();

        /** Implementation of appending a C-String at the index by the given size */
        constexpr void AppendImpl(const TChar* CString, Index size);
    };

    // ------------------------------------------------------------------------------------------------
    // Non-Member functions
    // ------------------------------------------------------------------------------------------------
    template<StringLiteral TChar, class TAllocator>
    constexpr String_Base<TChar, TAllocator> operator+(const String_Base<TChar, TAllocator>& string, const TChar* CString)
    {
        String_Base<TChar> str = string;
        str.Append(CString);
        return str;
    }

    template<StringLiteral TChar, class TAllocator>
    constexpr String_Base<TChar, TAllocator> operator+(String_Base<TChar, TAllocator>&& string, const TChar* CString)
    {
        String_Base<TChar> str = Move(string);
        str.Append(CString);
        return str;
    }

    template<StringLiteral TChar, class TAllocator>
    constexpr String_Base<TChar, TAllocator> operator+(const String_Base<TChar, TAllocator>& string, const String_Base<TChar>& otherString)
    {
        String_Base<TChar> str = string;
        str.Append(otherString);
        return str;
    }

    template<StringLiteral TChar, class TAllocator>
    constexpr String_Base<TChar, TAllocator> operator+(String_Base<TChar, TAllocator>&& string, const String_Base<TChar>& otherString)
    {
        String_Base<TChar> str = Move(string);
        str.Append(otherString);
        return str;
    }

    template<StringLiteral TChar>
    constexpr String_Base<TChar> operator+(const TChar* CString, const String_Base<TChar>& string)
    {
        return jpt::String_Base<TChar>(CString) += string;
    }

    template<StringLiteral TChar>
    constexpr String_Base<TChar> operator+(const TChar* CString, String_Base<TChar>&& rightString)
    {
        return jpt::String_Base<TChar>(CString) += Move(rightString);
    }

    template<StringLiteral TChar, class TAllocator>
    constexpr bool operator==(const String_Base<TChar, TAllocator>& string, const TChar* CString)
    {
        return AreStringsSame(string.ConstBuffer(), CString, string.Count(), FindCharsCount(CString));
    }

    /** Comparing a String class with raw CStr but different char type */
    template<StringLiteral TChar1, class TAllocator, StringLiteral TChar2 = TChar1>
    constexpr bool operator==(const String_Base<TChar1, TAllocator>& string, const TChar2* CString)
    {
        const Index cStrCount = FindCharsCount(CString);
        bool result = false;

        // String to const wchar_t*
        if constexpr (AreSameType<TChar1, char> && AreSameType<TChar2, wchar_t>)
        {
            const char* pCStr = ToChars(CString, cStrCount);
            result = AreStringsSame(string.ConstBuffer(), pCStr, string.Count(), cStrCount);
            JPT_DELETE_ARRAY(pCStr);
        }
        // WString to const char*
        else if constexpr (AreSameType<TChar1, wchar_t> && AreSameType<TChar2, char>)
        {
            const wchar_t* pWCStr = ToWChars(CString, cStrCount);
            result = AreStringsSame(string.ConstBuffer(), pWCStr, string.Count(), cStrCount);
            JPT_DELETE_ARRAY(pWCStr);
        }
        else
        {
            JPT_ASSERT(false, "Unsupported type");
        }

        return result;
    }

    template<StringLiteral TChar, class TAllocator>
    constexpr bool operator==(const String_Base<TChar, TAllocator>& lhs, const String_Base<TChar>& rhs)
    {
        return AreStringsSame(lhs.ConstBuffer(), rhs.ConstBuffer(), lhs.Count(), rhs.Count());
    }

    template<StringLiteral TChar, class TAllocator>
    constexpr uint64 Hash(const String_Base<TChar, TAllocator>& str)
    {
        return StringHash64(str.ConstBuffer());
    }

    // ------------------------------------------------------------------------------------------------
    // Member Functions
    // ------------------------------------------------------------------------------------------------
    template<StringLiteral TChar, class TAllocator>
    constexpr String_Base<TChar, TAllocator>::String_Base(const TChar* CString, Index size)
    {
        CopyString(CString, size);
    }

    template<StringLiteral TChar, class TAllocator>
    constexpr String_Base<TChar, TAllocator>::String_Base(const TChar* CString)
    {
        CopyString(CString);
    }

    template<StringLiteral TChar, class TAllocator>
    constexpr String_Base<TChar, TAllocator>::String_Base(TChar c)
    {
        TChar* pBuffer = TAllocator::NewArray(2);
        pBuffer[0] = c;
        pBuffer[1] = '\0';
        MoveString(pBuffer, 1);
    }

    template<StringLiteral TChar, class TAllocator>
    constexpr String_Base<TChar, TAllocator>::String_Base(const String_Base<TChar>& otherString)
    {
        CopyString(otherString);
    }

    template<StringLiteral TChar, class TAllocator>
    constexpr String_Base<TChar, TAllocator>::String_Base(String_Base<TChar>&& otherString) noexcept
    {
        MoveString(Move(otherString));
    }

    template<StringLiteral TChar, class TAllocator>
    String_Base<TChar, TAllocator>& String_Base<TChar, TAllocator>::operator=(const TChar* CString)
    {
        CopyString(CString);
        return *this;
    }

    template<StringLiteral TChar, class TAllocator>
    String_Base<TChar, TAllocator>& String_Base<TChar, TAllocator>::operator=(const String_Base<TChar>& otherString)
    {
        if (this != &otherString)
        {
            CopyString(otherString);
        }

        return *this;
    }

    template<StringLiteral TChar, class TAllocator>
    String_Base<TChar, TAllocator>& String_Base<TChar, TAllocator>::operator=(String_Base<TChar>&& otherString) noexcept
    {
        if (this != &otherString)
        {
            MoveString(Move(otherString));
        }

        return *this;
    }

    template<StringLiteral TChar, class TAllocator>
    constexpr String_Base<TChar, TAllocator>::~String_Base()
    {
        Clear();
    }

    template<StringLiteral TChar, class TAllocator>
    constexpr const TChar* String_Base<TChar, TAllocator>::ConstBuffer() const noexcept
    {
        return m_pBuffer;
    }

    template<StringLiteral TChar, class TAllocator>
    constexpr TChar* String_Base<TChar, TAllocator>::Buffer() const noexcept
    {
        return m_pBuffer;
    }

    template<StringLiteral TChar, class TAllocator>
    constexpr TChar& String_Base<TChar, TAllocator>::Front() noexcept
    {
        return m_pBuffer[0];
    }

    template<StringLiteral TChar, class TAllocator>
    constexpr const TChar& String_Base<TChar, TAllocator>::Front() const noexcept
    {
        return m_pBuffer[0];
    }

    template<StringLiteral TChar, class TAllocator>
    constexpr TChar& String_Base<TChar, TAllocator>::Back() noexcept
    {
        return m_pBuffer[m_count - 1];
    }

    template<StringLiteral TChar, class TAllocator>
    constexpr const TChar& String_Base<TChar, TAllocator>::Back() const noexcept
    {
        return m_pBuffer[m_count - 1];
    }

    template<StringLiteral TChar, class TAllocator>
    constexpr TChar& String_Base<TChar, TAllocator>::operator[](Index index) noexcept
    {
        JPT_ASSERT(index < m_count);
        return m_pBuffer[index];
    }

    template<StringLiteral TChar, class TAllocator>
    constexpr const TChar& String_Base<TChar, TAllocator>::operator[](Index index) const noexcept
    {
        JPT_ASSERT(index < m_count);
        return m_pBuffer[index];
    }

    template<StringLiteral TChar, class TAllocator>
    constexpr String_Base<TChar, TAllocator>::Iterator String_Base<TChar, TAllocator>::begin() noexcept
    {
        return Iterator(m_pBuffer);
    }

    template<StringLiteral TChar, class TAllocator>
    constexpr String_Base<TChar, TAllocator>::Iterator String_Base<TChar, TAllocator>::end() noexcept
    {
        return Iterator(m_pBuffer + m_count);
    }

    template<StringLiteral TChar, class TAllocator>
    constexpr String_Base<TChar, TAllocator>::ConstIterator String_Base<TChar, TAllocator>::begin() const noexcept
    {
        return ConstIterator(m_pBuffer);
    }

    template<StringLiteral TChar, class TAllocator>
    constexpr String_Base<TChar, TAllocator>::ConstIterator String_Base<TChar, TAllocator>::cbegin() const noexcept
    {
        return ConstIterator(m_pBuffer);
    }

    template<StringLiteral TChar, class TAllocator>
    constexpr String_Base<TChar, TAllocator>::ConstIterator String_Base<TChar, TAllocator>::end() const noexcept
    {
        return ConstIterator(m_pBuffer + m_count);
    }

    template<StringLiteral TChar, class TAllocator>
    constexpr String_Base<TChar, TAllocator>::ConstIterator String_Base<TChar, TAllocator>::cend() const noexcept
    {
        return ConstIterator(m_pBuffer + m_count);
    }

    template<StringLiteral TChar, class TAllocator>
    constexpr Index String_Base<TChar, TAllocator>::Count() const noexcept
    {
        return m_count;
    }

    template<StringLiteral TChar, class TAllocator>
    constexpr Index String_Base<TChar, TAllocator>::Count(TChar c, Index startIndex /* = 0*/, Index endIndex /* = kInvalidIndex*/) const noexcept
    {
        endIndex = Clamp(endIndex, Index(0), m_count);
        Index count = 0;

        for (Index i = startIndex; i < endIndex; ++i)
        {
            const TChar ch = m_pBuffer[i];
            if (ch == c)
            {
                ++count;
            }
        }

        return count;
    }

    template<StringLiteral TChar, class TAllocator>
    constexpr Index String_Base<TChar, TAllocator>::Count(const TChar* pString, Index startIndex /* = 0*/, Index endIndex /* = kInvalidIndex*/) const noexcept
    {
        endIndex = Clamp(endIndex, Index(0), m_count);
        Index count = 0;
        const Index stringToFindSize = FindCharsCount(pString);

        for (Index i = startIndex; i < endIndex;)
        {
            if ((i + stringToFindSize) > endIndex)
            {
                break;
            }

            if (AreStringsSame(m_pBuffer + i, pString, stringToFindSize))
            {
                ++count;
                i += stringToFindSize;
            }
            else
            {
                ++i;
            }
        }

        return count;
    }

    template<StringLiteral TChar, class TAllocator>
    constexpr Index String_Base<TChar, TAllocator>::Size() const noexcept
    {
        return m_count * sizeof(TChar);
    }

    template<StringLiteral TChar, class TAllocator>
    constexpr Index String_Base<TChar, TAllocator>::Capacity() const noexcept
    {
        return m_capacity;
    }

    template<StringLiteral TChar, class TAllocator>
    constexpr bool String_Base<TChar, TAllocator>::IsEmpty() const noexcept
    {
        return m_count == 0;
    }

    template<StringLiteral TChar, class TAllocator>
    constexpr Index String_Base<TChar, TAllocator>::Find(TChar charToFind, Index startIndex /* = 0*/, Index endIndex /* = kInvalidIndex*/) const noexcept
    {
        endIndex = Clamp(endIndex, Index(0), m_count);

        for (Index i = startIndex; i < endIndex; ++i)
        {
            if (m_pBuffer[i] == charToFind)
            {
                return i;
            }
        }

        return kInvalidIndex;
    }

    template<StringLiteral TChar, class TAllocator>
    constexpr Index String_Base<TChar, TAllocator>::Find(const TChar* pStringToFind, Index startIndex /*= 0*/, Index endIndex/*= kInvalidIndex*/) const noexcept
    {
        const Index stringToFindSize = FindCharsCount(pStringToFind);
        endIndex = Clamp(endIndex, static_cast<Index>(0), m_count);

        for (Index i = startIndex; i < endIndex; ++i)
        {
            if ((i + stringToFindSize) > endIndex)
            {
                return kInvalidIndex;
            }

            if (AreStringsSame(m_pBuffer + i, pStringToFind, stringToFindSize))
            {
                return i;
            }
        }

        return kInvalidIndex;
    }

    template<StringLiteral TChar, class TAllocator>
    constexpr Index String_Base<TChar, TAllocator>::FindFirstOf(TChar charToFind, Index startIndex /*= 0*/, Index endIndex/*= kInvalidIndex*/) const noexcept
    {
        return Find(charToFind, startIndex, endIndex);
    }

    template<StringLiteral TChar, class TAllocator>
    constexpr Index String_Base<TChar, TAllocator>::FindFirstOf(const TChar* pStringToFind, Index startIndex /*= 0*/, Index endIndex/*= kInvalidIndex*/) const noexcept
    {
        return Find(pStringToFind, startIndex, endIndex);
    }

    template<StringLiteral TChar, class TAllocator>
    constexpr Index String_Base<TChar, TAllocator>::FindLastOf(TChar charToFind, Index startIndex /*= 0*/, Index endIndex/*= kInvalidIndex*/) const noexcept
    {
        endIndex = Clamp(endIndex, Index(0), m_count);

        for (int64 i = endIndex; i >= static_cast<int64>(startIndex); --i)
        {
            if (i < static_cast<int64>(startIndex))
            {
                return kInvalidIndex;
            }

            if (m_pBuffer[i] == charToFind)
            {
                return i;
            }
        }

        return kInvalidIndex;
    }

    template<StringLiteral TChar, class TAllocator>
    constexpr Index String_Base<TChar, TAllocator>::FindLastOf(const TChar* pStringToFind, Index startIndex /*= 0*/, Index endIndex/*= kInvalidIndex*/) const noexcept
    {
        const Index StringToFindSize = FindCharsCount(pStringToFind);
        endIndex = Clamp(endIndex, Index(0), m_count);

        for (int64 i = endIndex; i >= static_cast<int64>(startIndex); --i)
        {
            if ((i - StringToFindSize) < startIndex)
            {
                return kInvalidIndex;
            }

            if (AreStringsSame(m_pBuffer + i - StringToFindSize, pStringToFind, StringToFindSize))
            {
                return i - StringToFindSize;
            }
        }

        return kInvalidIndex;
    }

    template<StringLiteral TChar, class TAllocator>
    constexpr bool String_Base<TChar, TAllocator>::Has(TChar charToFind, Index startIndex, Index endIndex) const noexcept
    {
        return Find(charToFind, startIndex, endIndex) != kInvalidIndex;
    }

    template<StringLiteral TChar, class TAllocator>
    constexpr bool String_Base<TChar, TAllocator>::Has(const TChar* pStringToFind, Index startIndex, Index endIndex) const noexcept
    {
        return Find(pStringToFind, startIndex, endIndex) != kInvalidIndex;
    }

    template<StringLiteral TChar, class TAllocator>
    constexpr bool String_Base<TChar, TAllocator>::BeginsWith(const TChar* pStringToFind) const noexcept
    {
        return Find(pStringToFind) == 0;
    }

    template<StringLiteral TChar, class TAllocator>
    constexpr bool String_Base<TChar, TAllocator>::EndsWith(const TChar* pStringToFind) const noexcept
    {
        const Index stringToFindSize = FindCharsCount(pStringToFind);
        const Index startIndex = m_count - stringToFindSize;
        const Index foundLastPos = FindLastOf(pStringToFind);
        return foundLastPos == startIndex;
    }

    template<StringLiteral TChar, class TAllocator>
    constexpr void String_Base<TChar, TAllocator>::Clear()
    {
        DeallocateBuffer();
        m_pBuffer = nullptr;
        m_count = 0;
        m_capacity = 0;
    }

    template<StringLiteral TChar, class TAllocator>
    constexpr String_Base<TChar, TAllocator>& String_Base<TChar, TAllocator>::Replace(const TChar* pStringToFind, const TChar* pStringToReplace, Index startIndex, Index endIndex)
    {
        if (endIndex == kInvalidIndex)
        {
            endIndex = m_count;
        }

        const Index stringToReplaceSize = FindCharsCount(pStringToReplace);
        const Index stringToFindSize = FindCharsCount(pStringToFind);

        // If the replaced string is larger than the string to find, we need to reserve more space
        if (stringToReplaceSize > stringToFindSize)
        {
            const Index count = Count(pStringToFind, startIndex, endIndex);
            Reserve(m_count + (stringToReplaceSize - stringToFindSize) * count);
        }

        while (true)
        {
            const Index foundPos = Find(pStringToFind, startIndex, endIndex);
            if (foundPos == kInvalidIndex)
            {
                break;
            }

            // Move the suffix to the destination
            void* pDestinationToMove  = m_pBuffer + foundPos + stringToReplaceSize;
            const void* pSourceToMove = m_pBuffer + foundPos + stringToFindSize;
            const Index sizeToMove   = (m_count - foundPos - stringToFindSize + 1) * sizeof(TChar);
            memmove(pDestinationToMove, pSourceToMove, sizeToMove);

            // Insert the string
            void* pDestinationToInsert = m_pBuffer + foundPos;
            const Index sizeToInsert  = stringToReplaceSize * sizeof(TChar);
            memcpy(pDestinationToInsert, pStringToReplace, sizeToInsert);

            // Update count and index for the next search
            startIndex = foundPos + stringToReplaceSize;    // In case 'StringToReplace' Has 'StringToFind', like replacing 'x' with 'yx'        
            const int64 offset = static_cast<int64>(stringToReplaceSize) - static_cast<int64>(stringToFindSize);
            endIndex += offset;
            m_count  += offset;
        }

        return *this;
    }

    template<StringLiteral TChar, class TAllocator>
    constexpr DynamicArray<String_Base<TChar, TAllocator>> String_Base<TChar, TAllocator>::Split(const TChar* pKeyword) const
    {
        DynamicArray<String_Base> substrs;
        String_Base copy = *this;
        const Index pKeywordSize = FindCharsCount(pKeyword);

        while (true)
        {
            const Index keywordIndex = copy.Find(pKeyword);
            if (keywordIndex == kInvalidIndex)
            {
                substrs.EmplaceBack(copy);
                break;
            }

            substrs.EmplaceBack(copy.SubStr(0, keywordIndex));
            copy = copy.SubStr(keywordIndex + pKeywordSize);
        }

        return substrs;
    }

    template<StringLiteral TChar, class TAllocator>
    constexpr DynamicArray<String_Base<TChar, TAllocator>> String_Base<TChar, TAllocator>::Split(TChar keyword) const
    {
        DynamicArray<String_Base> substrs;
        String_Base copy = *this;

        while (true)
        {
            const Index keywordIndex = copy.Find(keyword);
            if (keywordIndex == kInvalidIndex)
            {
                substrs.EmplaceBack(copy);
                break;
            }

            substrs.EmplaceBack(copy.SubStr(0, keywordIndex));
            copy = copy.SubStr(keywordIndex + 1);
        }

        return substrs;
    }

    template<StringLiteral TChar, class TAllocator>
    constexpr String_Base<TChar, TAllocator> String_Base<TChar, TAllocator>::SubStr(Index index, Index count /*= kInvalidIndex*/) const
    {
        if (count == kInvalidIndex)
        {
            count = m_count - index;
        }

        JPT_ASSERT((index + count) <= m_count, "SubStr cannot exceeds string's bound");
        
        if (count == 0)
        {
            return String_Base<TChar>();
        }

        String_Base<TChar> result;

        if (count < kSmallDataSize)
        {
            StrNCpy(result.m_smallBuffer, kSmallDataSize, &m_pBuffer[index], count);
            result.m_smallBuffer[count] = '\0';
            result.m_pBuffer = result.m_smallBuffer;
            result.m_count = count;
            result.m_capacity = kSmallDataSize - 1;
        }
        else
        {
            result.m_pBuffer = TAllocator::NewArray(count + 1);
            StrNCpy(result.m_pBuffer, count + 1, &m_pBuffer[index], count);

            result.m_pBuffer[count] = '\0';
            result.m_count = count;
            result.m_capacity = count;
        }

        return result;
    }

    template<StringLiteral TChar, class TAllocator>
    constexpr void String_Base<TChar, TAllocator>::PopBack(Index count /*= 1*/)
    {
        Resize(m_count - count);
    }

    template<StringLiteral TChar, class TAllocator>
    constexpr void String_Base<TChar, TAllocator>::TrimLeft(Index index /* = kInvalidIndex*/)
    {
        // If index == kInvalidIndex, trim all the white spaces from the left
        if (index == kInvalidIndex)
        {
            Index i = 0;
            while (i < m_count && m_pBuffer[i] == ' ')
            {
                ++i;
            }

            index = i;
        }

        // Trim from the left to the index
        JPT_ASSERT(index <= m_count, "Index out of bound");
        *this = SubStr(index);
    }

    template<StringLiteral TChar, class TAllocator>
    constexpr void String_Base<TChar, TAllocator>::TrimRight(Index index /* = kInvalidIndex*/)
    {
        // If index == kInvalidIndex, trim all the white spaces from the right
        if (index == kInvalidIndex)
        {
            Index i = m_count - 1;
            while (i >= 0 && m_pBuffer[i] == ' ')
            {
                --i;
            }

            index = i + 1;
        }

        // Trim from right to the index
        JPT_ASSERT(index <= m_count, "Index out of bound");
        m_pBuffer[index] = '\0';
        m_count = index;
    }

    template<StringLiteral TChar, class TAllocator>
    constexpr void String_Base<TChar, TAllocator>::Insert(TChar c, Index index)
    {
        TChar cString[2] = { c, '\0' };
        Insert(cString, index, 1);
    }

    template<StringLiteral TChar, class TAllocator>
    constexpr void String_Base<TChar, TAllocator>::Insert(const TChar* CString, Index index)
    {
        Insert(CString, index, FindCharsCount(CString));
    }

    template<StringLiteral TChar, class TAllocator>
    constexpr void String_Base<TChar, TAllocator>::Insert(const TChar* CString, Index index, Index size)
    {
        JPT_ASSERT(index <= m_count, "Index out of bound");
        if (size == 0)
        {
            return;
        }

        Reserve(m_count + size);

        // Move the suffix to the destination
        void* pDestinationToMove  = m_pBuffer + index + size;
        const void* pSourceToMove = m_pBuffer + index;
        const Index sizeToMove   = (m_count - index + 1) * sizeof(TChar);
        memmove(pDestinationToMove, pSourceToMove, sizeToMove);

        // Insert the string
        void* pDestinationToInsert = m_pBuffer + index;
        const Index sizeToInsert  = size * sizeof(TChar);
        memcpy(pDestinationToInsert, CString, sizeToInsert);

        m_count += size;
    }

    template<StringLiteral TChar, class TAllocator>
    constexpr void String_Base<TChar, TAllocator>::Append(const TChar* CString, Index newStringSize)
    {
        if (newStringSize == 0)
        {
            return;
        }

        AppendImpl(CString, newStringSize);
    }

    template<StringLiteral TChar, class TAllocator>
    constexpr void String_Base<TChar, TAllocator>::Append(const TChar* CString)
    {
        Append(CString, FindCharsCount(CString));
    }

    template<StringLiteral TChar, class TAllocator>
    constexpr void String_Base<TChar, TAllocator>::Append(const String_Base<TChar>& otherString)
    {
        if (otherString.IsEmpty())
        {
            return;
        }

        AppendImpl(otherString.ConstBuffer(), otherString.m_count);
    }

    template<StringLiteral TChar, class TAllocator>
    constexpr void String_Base<TChar, TAllocator>::Append(const DynamicArray<String_Base<TChar>>& strings, const TChar* separator)
    {
        Index newCount = m_count;
        const Index separatorCount = FindCharsCount(separator);

        for (Index i = 0; i < strings.Count(); ++i)
        {
            newCount += strings[i].Count();
            if (i != strings.Count() - 1 && separator)
            {
                newCount += separatorCount;
            }
        }

        Reserve(newCount);

        for (Index i = 0; i < strings.Count(); ++i)
        {
            Append(strings[i]);
            if (i != strings.Count() - 1 && separator)
            {
                Append(separator, separatorCount);
            }
        }
    }

    template<StringLiteral TChar, class TAllocator>
    constexpr void String_Base<TChar, TAllocator>::Append(TChar c)
    {
        TChar cString[2] = { c, '\0' };
        AppendImpl(cString, 1);
    }

    template<StringLiteral TChar, class TAllocator>
    constexpr String_Base<TChar, TAllocator>& String_Base<TChar, TAllocator>::operator+=(const TChar* CString)
    {
        Append(CString);
        return *this;
    }

    template<StringLiteral TChar, class TAllocator>
    constexpr String_Base<TChar, TAllocator>& String_Base<TChar, TAllocator>::operator+=(const String_Base<TChar>& otherString)
    {
        Append(otherString);
        return *this;
    }

    template<StringLiteral TChar, class TAllocator>
    constexpr String_Base<TChar, TAllocator>& String_Base<TChar, TAllocator>::operator+=(TChar c)
    {
        Append(c);
        return *this;
    }

    template<StringLiteral TChar, class TAllocator>
    constexpr void String_Base<TChar, TAllocator>::Reserve(Index capacity)
    {
        if (capacity <= m_capacity)
        {
            return;    
        }

        if (capacity < kSmallDataSize)
        {
            DeallocateBuffer();
            m_pBuffer = m_smallBuffer;
            m_capacity = kSmallDataSize - 1;
        }
        else
        {
            TChar* pNewBuffer = TAllocator::NewArray(capacity + 1);

            // Copy the old buffer to the current one
            if (m_pBuffer)
            {
                StrCpy(pNewBuffer, m_count + 1, m_pBuffer);
                DeallocateBuffer();
            }

            m_pBuffer = pNewBuffer;
            m_capacity = capacity;
        }
    }

    template<StringLiteral TChar, class TAllocator>
    constexpr void String_Base<TChar, TAllocator>::Resize(Index newSize)
    {
        if (newSize == 0)
        {
            Clear();
            return;
        }

        if (newSize < m_count)
        {
            m_count = newSize;
        }
        else
        {
            Reserve(newSize);
            m_count = newSize;
        }

        m_pBuffer[m_count] = '\0';
    }

    template<StringLiteral TChar, class TAllocator>
    template<Index kCount>
    constexpr String_Base<TChar, TAllocator> String_Base<TChar, TAllocator>::Format(const TChar* format, ...)
    {
        TChar buffer[kCount];

        if constexpr (AreSameType<TChar, char>)
        {
            JPT_FORMAT_STRING(buffer, format, ...);
        }
        else if constexpr (AreSameType<TChar, wchar_t>)
        {
            JPT_FORMAT_WSTRING(buffer, format, ...);
        }

        return String_Base(buffer);
    }

    template<StringLiteral TChar, class TAllocator>
    constexpr void String_Base<TChar, TAllocator>::CopyString(const TChar* inCString, Index size)
    {
        if (AreStringsSame(m_pBuffer, inCString, m_count, size))
        {
            return;
        }

        if (size == m_count)
        {
            StrNCpy(m_pBuffer, size + 1, inCString, size);
            return;
        }

        DeallocateBuffer();

        if (size == 0)
        {
            m_pBuffer = nullptr;
        }
        else
        {
            if (size < kSmallDataSize)
            {
                StrNCpy(m_smallBuffer, size + 1, inCString, size);
                m_pBuffer = m_smallBuffer;
                m_capacity = kSmallDataSize - 1;
            }
            else 
            {
                m_pBuffer = TAllocator::NewArray(size + 1);
                StrNCpy(m_pBuffer, size + 1, inCString, size);
                m_capacity = size;
            }
        }

        m_count = size;
    }

    template<StringLiteral TChar, class TAllocator>
    constexpr void String_Base<TChar, TAllocator>::CopyString(const TChar* inCString)
    {
        CopyString(inCString, FindCharsCount(inCString));
    }

    template<StringLiteral TChar, class TAllocator>
    constexpr void String_Base<TChar, TAllocator>::CopyString(const String_Base<TChar>& otherString)
    {
        CopyString(otherString.ConstBuffer(), otherString.Count());
    }

    template<StringLiteral TChar, class TAllocator>
    constexpr void String_Base<TChar, TAllocator>::MoveString(TChar* inCString, Index size)
    {
        DeallocateBuffer();

        if (size == 0)
        {
            m_pBuffer = nullptr;
        }
        else if (size < kSmallDataSize)
        {
            StrNCpy(m_smallBuffer, size + 1, inCString, size);
            m_pBuffer = m_smallBuffer;
            TAllocator::DeleteArray(inCString);
            m_capacity = kSmallDataSize - 1;
        }
        else
        {
            m_pBuffer = inCString;
            m_capacity = size;
        }

        m_count     = size;
    }

    template<StringLiteral TChar, class TAllocator>
    constexpr void String_Base<TChar, TAllocator>::MoveString(TChar* inCString)
    {
        MoveString(inCString, FindCharsCount(inCString));
    }

    template<StringLiteral TChar, class TAllocator>
    constexpr void String_Base<TChar, TAllocator>::MoveString(String_Base<TChar>&& otherString)
    {
        DeallocateBuffer();

        if (otherString.IsEmpty())
        {
            m_pBuffer = nullptr;
        }
        else if (otherString.Count() < kSmallDataSize)
        {
            StrCpy(m_smallBuffer, otherString.m_count + 1, otherString.m_pBuffer);
            m_pBuffer = m_smallBuffer;
            otherString.DeallocateBuffer();
            m_capacity = kSmallDataSize - 1;
        }
        else
        {
            m_pBuffer = otherString.m_pBuffer;
            m_capacity = otherString.m_capacity;
        }

        m_count    = otherString.m_count;

        otherString.m_pBuffer  = nullptr;
        otherString.m_count     = 0;
        otherString.m_capacity = 0;
    }

    template<StringLiteral TChar, class TAllocator>
    void String_Base<TChar, TAllocator>::Serialize(Serializer& serializer) const
    {
        serializer.Write(m_count);
        serializer.Write(m_capacity);
        serializer.Write(reinterpret_cast<const char*>(m_smallBuffer), kSmallDataSize * sizeof(TChar));
        serializer.Write(reinterpret_cast<const char*>(m_pBuffer), m_count * sizeof(TChar));
    }

    template<StringLiteral TChar, class TAllocator>
    void String_Base<TChar, TAllocator>::Deserialize(Serializer& serializer)
    {
        Index count = 0;
        serializer.Read(count);

        Index capacity = 0;
        serializer.Read(capacity);

        Resize(count);
        serializer.Read(reinterpret_cast<char*>(m_smallBuffer), kSmallDataSize * sizeof(TChar));
        serializer.Read(reinterpret_cast<char*>(m_pBuffer), m_count * sizeof(TChar));
        m_count = count;
        m_capacity = capacity;
    }

    template<StringLiteral TChar, class TAllocator>
    constexpr void String_Base<TChar, TAllocator>::DeallocateBuffer()
    {
        if (m_pBuffer && 
            m_pBuffer != m_smallBuffer)
        {
            TAllocator::DeleteArray(m_pBuffer);
        }
    }

    template<StringLiteral TChar, class TAllocator>
    constexpr void String_Base<TChar, TAllocator>::AppendImpl(const TChar* CString, Index size)
    {
        if (size == 0)
        {
            return;
        }

        const Index newSize = m_count + size;

        Reserve(newSize);
        StrCpy(m_pBuffer + m_count, size + 1, CString);

        m_count = newSize;
    }

    using String = String_Base<char>;
    using WString = String_Base<wchar_t>;    // Wide string

    template<typename T>
    concept StringType = AreSameType<T, String> || AreSameType<T, WString>;

    template<typename T> constexpr bool IsStringLiteral = false;
    template<Index N>   constexpr bool IsStringLiteral<char[N]>          = true;
    template<Index N>   constexpr bool IsStringLiteral<wchar_t[N]>       = true;
    template<Index N>   constexpr bool IsStringLiteral<const char[N]>    = true;
    template<Index N>   constexpr bool IsStringLiteral<const wchar_t[N]> = true;
}