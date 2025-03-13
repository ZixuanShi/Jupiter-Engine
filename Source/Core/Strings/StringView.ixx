// Copyright Jupiter Technologies, Inc. All Rights Reserved.

module;

#include "Core/Validation/Assert.h"

export module jpt.StringView;

import jpt.Allocator;
import jpt.Concepts;
import jpt.Constants;
import jpt.Math;
import jpt.String;
import jpt.String.Helpers;
import jpt.TypeDefs;

export namespace jpt
{
    /** Describes an object that can refer to a constant contiguous sequence 
        of TChar with the first element of the sequence at position zero. */
    template<StringLiteral _TChar>
    class StringView_Base
    {
    public:
        using TChar = _TChar;

    private:
        const TChar* m_pBuffer = nullptr;
        size_t m_count = 0;

    public:
        // Constructors
        constexpr StringView_Base() = default;
        constexpr StringView_Base(const TChar* CString, size_t size);
        constexpr StringView_Base(const TChar* CString);
        constexpr StringView_Base(const StringView_Base& other);
        constexpr StringView_Base(const String_Base<TChar>& string);

        // operator=
        StringView_Base& operator=(const TChar* CString);
        StringView_Base& operator=(const StringView_Base& other);
        StringView_Base& operator=(const String_Base<TChar>& string);

        // Element Access
        constexpr const TChar* ConstBuffer()    const { return m_pBuffer;             } 
        constexpr const TChar& At(size_t index) const { return m_pBuffer[index];      }
        constexpr const TChar& Front()          const { return m_pBuffer[0];          }
        constexpr const TChar& Back()           const { return m_pBuffer[m_count - 1]; }
        constexpr const TChar& operator[](size_t index) const { return m_pBuffer[index]; }

        // Capacity
        constexpr size_t Count() const { return m_count; }
        constexpr bool IsEmpty() const { return m_count == 0; }

        /** @return        A sub string within the given range at index and length */
        constexpr StringView_Base SubStr(size_t index, size_t count = npos) const;

        /** @return     true if the string view starts with the given prefix */
        constexpr bool StartsWith(const TChar* CString, size_t count) const;
        constexpr bool StartsWith(const TChar* CString) const { return StartsWith(CString, FindCharsCount(CString)); }

        /** @return     true if the string view ends with the given suffix */
        constexpr bool EndsWith(const TChar* CString, size_t count) const;
        constexpr bool EndsWith(const TChar* CString) const { return EndsWith(CString, FindCharsCount(CString)); }

        constexpr size_t Find(      TChar  charToFind,    size_t startIndex = 0, size_t endIndex = npos, size_t count = 1) const;
        constexpr size_t Find(const TChar* pStringToFind, size_t startIndex = 0, size_t endIndex = npos, size_t count = 1) const;
        constexpr size_t FindLastOf(      TChar charToFind,     size_t startIndex = 0, size_t endIndex = npos, size_t count = 1)  const;
        constexpr size_t FindLastOf(const TChar* pStringToFind, size_t startIndex = 0, size_t endIndex = npos, size_t count = 1)  const;
        constexpr bool   Has(      TChar  charToFind, size_t startIndex = 0, size_t endIndex = npos, size_t count = 1) const { return Find(charToFind, startIndex, endIndex, count) != npos; }
        constexpr bool   Has(const TChar* charToFind, size_t startIndex = 0, size_t endIndex = npos, size_t count = 1) const { return Find(charToFind, startIndex, endIndex, count) != npos; }
    };

    // Non member functions -------------------------------------------------------------------------------------------------------------------
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

    // Member Functions Definitions --------------------------------------------------------------------------------------------------------
    template<StringLiteral TChar>
    constexpr StringView_Base<TChar>::StringView_Base(const TChar* CString, size_t size)
        : m_pBuffer(CString)
        , m_count(size)
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
    constexpr StringView_Base<TChar> StringView_Base<TChar>::SubStr(size_t index, size_t count) const
    {
        if (count == npos)
        {
            count = m_count - index;
        }

        JPT_ASSERT((index + count) <= m_count, "SubStr cannot exceeds string's bound");

        return StringView_Base(m_pBuffer + index, count);
    }

    template<StringLiteral _TChar>
    constexpr bool StringView_Base<_TChar>::StartsWith(const TChar* CString, size_t count) const
    {
        return AreStringsSame(m_pBuffer, CString, count);
    }

    template<StringLiteral _TChar>
    constexpr bool StringView_Base<_TChar>::EndsWith(const TChar* CString, size_t count) const
    {
        return AreStringsSame(m_pBuffer + m_count - count, CString, count);
    }

    template<StringLiteral _TChar>
    constexpr size_t StringView_Base<_TChar>::Find(TChar charToFind, size_t startIndex /* = 0*/, size_t endIndex /* = npos*/, size_t count/* = 1*/) const
    {
        ClampTo(endIndex, size_t(0), m_count);

        for (size_t i = startIndex; i < endIndex; ++i)
        {
            if ((i + 1) > endIndex)
            {
                return npos;
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

        return npos;
    }

    template<StringLiteral _TChar>
    constexpr size_t StringView_Base<_TChar>::Find(const TChar* pStringToFind, size_t startIndex /*= 0*/, size_t endIndex/*= npos*/, size_t count/* = 1*/) const
    {
        const size_t StringToFindSize = FindCharsCount(pStringToFind);
        ClampTo(endIndex, static_cast<size_t>(0), m_count);

        StringView_Base current;
        for (size_t i = startIndex; i < endIndex; ++i)
        {
            if ((i + StringToFindSize) > endIndex)
            {
                return npos;
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

        return npos;
    }

    template<StringLiteral _TChar>
    constexpr size_t StringView_Base<_TChar>::FindLastOf(TChar charToFind, size_t startIndex, size_t endIndex, size_t count) const
    {
        ClampTo(endIndex, size_t(0), m_count);

        for (int64 i = endIndex - 1; i >= static_cast<int64>(startIndex); --i)
        {
            if (i < static_cast<int64>(startIndex))
            {
                return npos;
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

        return npos;
    }

    template<StringLiteral _TChar>
    constexpr size_t StringView_Base<_TChar>::FindLastOf(const TChar* pStringToFind, size_t startIndex, size_t endIndex, size_t count) const
    {
        const size_t StringToFindSize = FindCharsCount(pStringToFind);
        ClampTo(endIndex, size_t(0), m_count);

        String_Base<TChar> current;
        for (int64 i = endIndex - 1; i >= static_cast<int64>(startIndex); --i)
        {
            if ((i - StringToFindSize) < startIndex)
            {
                return npos;
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

        return npos;
    }

    using StringView = StringView_Base<char>;
    using WStringView = StringView_Base<wchar_t>;
}