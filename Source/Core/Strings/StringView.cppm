// Copyright Jupiter Technologies, Inc. All Rights Reserved.

module;

#include "Core/Validation/Assert.h"
#include "Debugging/Logger.h"

export module jpt.StringView;

import jpt.Allocator;
import jpt.Concepts;
import jpt.Constants;
import jpt.Math;
import jpt.String;
import jpt.StringHelpers;
import jpt.TypeDefs;

export namespace jpt
{
    /** Describes an object that can refer to a constant contiguous sequence 
        of TChar with the first element of the sequence at position zero. */
    template<StringLiteral _TChar>
    class StringView_Base
    {
    public:
        using TChar         = _TChar;
        using Iterator      = TChar*;
        using ConstIterator = const TChar*;

    private:
        const TChar* m_pBuffer = nullptr;
        Index m_count = 0;

    public:
        // Constructors
        constexpr StringView_Base() = default;
        constexpr StringView_Base(const TChar* CString, Index count);
        constexpr StringView_Base(const TChar* CString);
        constexpr StringView_Base(const StringView_Base& other);
        constexpr StringView_Base(const String_Base<TChar>& string);

        // operator=
        StringView_Base& operator=(const TChar* CString);
        StringView_Base& operator=(const StringView_Base& other);
        StringView_Base& operator=(const String_Base<TChar>& string);

        // Element Access
        constexpr const TChar* ConstBuffer()    const { return m_pBuffer;               } 
        constexpr const TChar& At(Index index)  const { return m_pBuffer[index];        }
        constexpr const TChar& Front()          const { return m_pBuffer[0];            }
        constexpr const TChar& Back()           const { return m_pBuffer[m_count - 1];  }
        constexpr const TChar& operator[](Index index) const { return m_pBuffer[index]; }

        // Iterators
        constexpr Iterator begin() noexcept;
        constexpr Iterator end() noexcept;
        constexpr ConstIterator begin()  const noexcept;
        constexpr ConstIterator cbegin() const noexcept;
        constexpr ConstIterator end()    const noexcept;
        constexpr ConstIterator cend()   const noexcept;

        // Capacity
        constexpr Index Count() const { return m_count; }
        constexpr bool IsEmpty() const { return m_count == 0; }

        /** @return        A sub string within the given range at index and length */
        constexpr StringView_Base SubStr(Index index, Index count = kInvalidIndex) const;

        /** @return     true if the string view starts with the given prefix */
        constexpr bool StartsWith(const TChar* CString, Index count) const;
        constexpr bool StartsWith(const TChar* CString) const { return StartsWith(CString, FindCharsCount(CString)); }

        /** @return     true if the string view ends with the given suffix */
        constexpr bool EndsWith(const TChar* CString, Index count) const;
        constexpr bool EndsWith(const TChar* CString) const { return EndsWith(CString, FindCharsCount(CString)); }

        constexpr Index Find(      TChar  charToFind,    Index startIndex = 0, Index endIndex = kInvalidIndex, Index count = 1) const;
        constexpr Index Find(const TChar* pStringToFind, Index startIndex = 0, Index endIndex = kInvalidIndex, Index count = 1) const;
        constexpr Index FindLastOf(      TChar charToFind,     Index startIndex = 0, Index endIndex = kInvalidIndex, Index count = 1)  const;
        constexpr Index FindLastOf(const TChar* pStringToFind, Index startIndex = 0, Index endIndex = kInvalidIndex, Index count = 1)  const;
        constexpr bool   Has(      TChar  charToFind, Index startIndex = 0, Index endIndex = kInvalidIndex, Index count = 1) const { return Find(charToFind, startIndex, endIndex, count) != kInvalidIndex; }
        constexpr bool   Has(const TChar* charToFind, Index startIndex = 0, Index endIndex = kInvalidIndex, Index count = 1) const { return Find(charToFind, startIndex, endIndex, count) != kInvalidIndex; }
    };

    // ------------------------------------------------------------------------------------------------
    // Non member functions
    // ------------------------------------------------------------------------------------------------
    template<StringLiteral TChar>
    constexpr bool operator==(const StringView_Base<TChar>& a, const StringView_Base<TChar>& b)
    {
        return AreStringsSame(a.ConstBuffer(), b.ConstBuffer(), a.Count(), b.Count());
    }
    template<StringLiteral TChar>
    constexpr bool operator==(const StringView_Base<TChar>& a, const TChar* b)
    {
        return AreStringsSame(a.ConstBuffer(), b, a.Count(), FindCharsCount(b));
    }
    template<StringLiteral TChar>
    constexpr bool operator==(const StringView_Base<TChar>& a, const String_Base<TChar>& b)
    {
        return AreStringsSame(a.ConstBuffer(), b.ConstBuffer(), a.Count(), b.Count());
    }

    template<StringLiteral TChar>
    constexpr String ToString(const StringView_Base<TChar>& stringView)
    {
        JPT_WARN("Converting StringView \"%s\" to a string. This allocates duplicated memory.", stringView.ConstBuffer());
        return String(stringView.ConstBuffer(), stringView.Count());
    }

    // ------------------------------------------------------------------------------------------------
    // Member Functions Definitions
    // ------------------------------------------------------------------------------------------------
    template<StringLiteral TChar>
    constexpr StringView_Base<TChar>::StringView_Base(const TChar* CString, Index count)
        : m_pBuffer(CString)
        , m_count(count)
    {
    }

    template<StringLiteral TChar>
    constexpr StringView_Base<TChar>::StringView_Base(const TChar* CString)
        : StringView_Base(CString, FindCharsCount(CString))
    {
    }

    template<StringLiteral _TChar>
    constexpr StringView_Base<_TChar>::StringView_Base(const StringView_Base& other)
        : m_pBuffer(other.m_pBuffer)
        , m_count(other.m_count)
    {
    }

    template<StringLiteral TChar>
    constexpr StringView_Base<TChar>::StringView_Base(const String_Base<TChar>& string)
        : m_pBuffer(string.ConstBuffer())
        , m_count(string.Count())
    {
    }

    template<StringLiteral TChar>
    StringView_Base<TChar>& StringView_Base<TChar>::operator=(const TChar* CString)
    {
        if (m_pBuffer != CString)
        {
            m_pBuffer = CString;
            m_count = FindCharsCount(CString);
        }

        return *this;
    }

    template<StringLiteral TChar>
    StringView_Base<TChar>& StringView_Base<TChar>::operator=(const StringView_Base& other)
    {
        if (this != &other)
        {
            m_pBuffer = other.m_pBuffer;
            m_count = other.m_count;
        }

        return *this;
    }

    template<StringLiteral TChar>
    StringView_Base<TChar>& StringView_Base<TChar>::operator=(const String_Base<TChar>& string)
    {
        if (m_pBuffer != string.ConstBuffer())
        {
            m_pBuffer = string.ConstBuffer();
            m_count = string.Count();
        }

        return *this;
    }

    template<StringLiteral TChar>
    constexpr StringView_Base<TChar>::Iterator StringView_Base<TChar>::begin() noexcept
    {
        return Iterator(m_pBuffer);
    }

    template<StringLiteral TChar>
    constexpr StringView_Base<TChar>::Iterator StringView_Base<TChar>::end() noexcept
    {
        return Iterator(m_pBuffer + m_count);
    }

    template<StringLiteral TChar>
    constexpr StringView_Base<TChar>::ConstIterator StringView_Base<TChar>::begin() const noexcept
    {
        return ConstIterator(m_pBuffer);
    }

    template<StringLiteral TChar>
    constexpr StringView_Base<TChar>::ConstIterator StringView_Base<TChar>::cbegin() const noexcept
    {
        return ConstIterator(m_pBuffer);
    }

    template<StringLiteral TChar>
    constexpr StringView_Base<TChar>::ConstIterator StringView_Base<TChar>::end() const noexcept
    {
        return ConstIterator(m_pBuffer + m_count);
    }

    template<StringLiteral TChar>
    constexpr StringView_Base<TChar>::ConstIterator StringView_Base<TChar>::cend() const noexcept
    {
        return ConstIterator(m_pBuffer + m_count);
    }

    template<StringLiteral TChar>
    constexpr StringView_Base<TChar> StringView_Base<TChar>::SubStr(Index index, Index count) const
    {
        if (count == kInvalidIndex)
        {
            count = m_count - index;
        }

        JPT_ASSERT((index + count) <= m_count, "SubStr cannot exceeds string's bound");

        return StringView_Base(m_pBuffer + index, count);
    }

    template<StringLiteral _TChar>
    constexpr bool StringView_Base<_TChar>::StartsWith(const TChar* CString, Index count) const
    {
        return AreStringsSame(m_pBuffer, CString, count);
    }

    template<StringLiteral _TChar>
    constexpr bool StringView_Base<_TChar>::EndsWith(const TChar* CString, Index count) const
    {
        return AreStringsSame(m_pBuffer + m_count - count, CString, count);
    }

    template<StringLiteral _TChar>
    constexpr Index StringView_Base<_TChar>::Find(TChar charToFind, Index startIndex /* = 0*/, Index endIndex /* = kInvalidIndex*/, Index count/* = 1*/) const
    {
        endIndex = Clamp(endIndex, Index(0), m_count);

        for (Index i = startIndex; i < endIndex; ++i)
        {
            if ((i + 1) > endIndex)
            {
                return kInvalidIndex;
            }

            if (m_pBuffer[i] == charToFind)
            {
                --count;
                if (count == 0)
                {
                    return i;
                }
            }
        }

        return kInvalidIndex;
    }

    template<StringLiteral _TChar>
    constexpr Index StringView_Base<_TChar>::Find(const TChar* pStringToFind, Index startIndex /*= 0*/, Index endIndex/*= kInvalidIndex*/, Index count/* = 1*/) const
    {
        const Index StringToFindSize = FindCharsCount(pStringToFind);
        endIndex = Clamp(endIndex, static_cast<Index>(0), m_count);

        StringView_Base current;
        for (Index i = startIndex; i < endIndex; ++i)
        {
            if ((i + StringToFindSize) > endIndex)
            {
                return kInvalidIndex;
            }

            current = SubStr(i, StringToFindSize);
            if (current == pStringToFind)
            {
                --count;
                if (count == 0)
                {
                    return i;
                }
            }
        }

        return kInvalidIndex;
    }

    template<StringLiteral _TChar>
    constexpr Index StringView_Base<_TChar>::FindLastOf(TChar charToFind, Index startIndex, Index endIndex, Index count) const
    {
        endIndex = Clamp(endIndex, Index(0), m_count);

        for (int64 i = endIndex - 1; i >= static_cast<int64>(startIndex); --i)
        {
            if (i < static_cast<int64>(startIndex))
            {
                return kInvalidIndex;
            }

            if (m_pBuffer[i] == charToFind)
            {
                --count;
                if (count == 0)
                {
                    return i;
                }
            }
        }

        return kInvalidIndex;
    }

    template<StringLiteral _TChar>
    constexpr Index StringView_Base<_TChar>::FindLastOf(const TChar* pStringToFind, Index startIndex, Index endIndex, Index count) const
    {
        const Index StringToFindSize = FindCharsCount(pStringToFind);
        endIndex = Clamp(endIndex, Index(0), m_count);

        String_Base<TChar> current;
        for (int64 i = endIndex - 1; i >= static_cast<int64>(startIndex); --i)
        {
            if ((i - StringToFindSize) < startIndex)
            {
                return kInvalidIndex;
            }

            current = SubStr(i - StringToFindSize, StringToFindSize);
            if (AreStringsSame(current.ConstBuffer(), pStringToFind, current.Count(), StringToFindSize))
            {
                --count;
                if (count == 0)
                {
                    return i - StringToFindSize;
                }
            }
        }

        return kInvalidIndex;
    }

    using StringView = StringView_Base<char>;
    using WStringView = StringView_Base<wchar_t>;

    template<typename T>
    concept StringViewType = AreSameType<T, StringView> || AreSameType<T, WStringView>;
}