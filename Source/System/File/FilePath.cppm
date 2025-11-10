// Copyright Jupiter Technologies, Inc. All Rights Reserved.

module;

#include "Core/Minimal/Utilities.h"
#include "Core/Validation/Assert.h"

export module jpt.FilePath;

import jpt.String;
import jpt.StringHelpers;
import jpt.ToString;
import jpt.TypeDefs;
import jpt.Utilities;

import jpt.File_Enums;
import jpt.FilePathHelpers;

export namespace jpt
{
    namespace File
    {
        /** Identifies path of a file */
        template<typename _TChar>
        class TPath
        {
            static_assert(AreSameType<_TChar, char> || AreSameType<_TChar, wchar_t>, "TPath must be either char or wchar_t");

        public:
            using TChar   = _TChar;
            using TString = String_Base<TChar>;

        private:
            TString m_path;

        public:
            constexpr TPath() = default;
            constexpr TPath(const char* path);
            constexpr TPath(const wchar_t* path);
            constexpr TPath(const TString& path);

            // FilePath Helpers
        public:
            /** @return     The parent folder of current path
                @example    File::Path("C:/Jupiter-Engine/Source").GetParent()          -> File::Path("C:/Jupiter-Engine/") 
                @example    File::Path("C:/Jupiter-Engine/Source/").GetParent()         -> File::Path("C:/Jupiter-Engine/") 
                @example    File::Path("C:/Jupiter-Engine/Source/Main.cpp").GetParent() -> File::Path("C:/Jupiter-Engine/Source/") */
            constexpr TPath GetParent() const;

            /** @return     The name of current directory or file
                @example    File::Path("C:/Jupiter-Engine/Source").GetName()          -> File::Path("Source") 
                @example    File::Path("C:/Jupiter-Engine/Source/").GetName()         -> File::Path("Source") 
                @example    File::Path("C:/Jupiter-Engine/Source/Main.cpp").GetName() -> File::Path("Main.cpp") */
            constexpr TPath GetName() const;

            template<typename T = TChar>
            constexpr String_Base<T> GetString() const noexcept;

        public:
            // Wrappers of TString
            
            // Element Access
            constexpr const TString& GetTString() const noexcept;
            constexpr const TChar* ConstBuffer() const noexcept;

            // Capacity
            constexpr size_t Count() const;
            constexpr bool IsEmpty() const;
            constexpr void Reserve(size_t size);

            // Modifiers
            constexpr void Append(const TPath& path);
            constexpr void operator+=(const WString& path);
            constexpr TPath& Replace(const TChar* StringToFind, const TChar* StringToReplace, size_t startIndex = 0, size_t endIndex = kInvalidIndex);

            // Search
            constexpr bool Has(const TPath& path) const;
            constexpr bool EndsWith(const TPath& path) const;
        };

#if IS_PLATFORM_WINDOWS || IS_PLATFORM_XBOX        
        using Path = TPath<wchar_t>;
#elif
        using Path = TPath<char>;
#endif
        
        constexpr bool operator==(const File::Path& lhs, const File::Path& rhs)
        {
            return AreStringsSame(lhs.ConstBuffer(), rhs.ConstBuffer());
        }

        constexpr File::Path operator+(const File::Path& lhs, const File::Path& rhs)
        {
            File::Path result = lhs;
            result.Append(rhs);
            return result;
        }

        constexpr uint64 Hash(const Path& path)
        {
            return Hash(path.GetTString());
        }

        // ------------------------------------------------------------------------------------------------
        // Member Functions
        // ------------------------------------------------------------------------------------------------
        template<typename TChar>
        constexpr TPath<TChar>::TPath(const char* path)
        {
            if constexpr (AreSameType<TChar, char>)
            {
                m_path = path;
            }
            else if constexpr (AreSameType<TChar, wchar_t>)
            {
                m_path = CStrToWStr(path);
            }

            FixSeparators(m_path);
        }

        template<typename TChar>
        constexpr TPath<TChar>::TPath(const wchar_t* path)
        {
            if constexpr (AreSameType<TChar, wchar_t>)
            {
                m_path = path;
            }
            else if constexpr (AreSameType<TChar, char>)
            {
                m_path = WCStrToStr(path);
            }

            FixSeparators(m_path);
        }

        template<typename TChar>
        constexpr TPath<TChar>::TPath(const TString& path)
            : m_path(path)
        {
            FixSeparators(m_path);
        }

        template<typename TChar>
        constexpr TPath<TChar> TPath<TChar>::GetParent() const
        {
            TString path = GetTString();

            const TString separator = GetSeparator<TString>();
            const size_t separatorLen = separator.Count();
            if (EndsWith(separator))
            {
                path.PopBack(separatorLen);
            }

            const size_t lastSeparatorIndex = path.FindLastOf(separator.ConstBuffer());
            if (lastSeparatorIndex == kInvalidIndex)
            {
                JPT_ASSERT(false, "Path has no parent");
                return {};
            }

            return path.SubStr(0, lastSeparatorIndex + separatorLen);
        }

        template<typename TChar>
        constexpr TPath<TChar> TPath<TChar>::GetName() const
        {
            TString path = GetTString();

            const TString separator = GetSeparator<TString>();
            const size_t separatorLen = separator.Count();
            if (EndsWith(separator))
            {
                path.PopBack(separatorLen);
            }

            const size_t lastSeparatorIndex = path.FindLastOf(separator.ConstBuffer());
            if (lastSeparatorIndex == kInvalidIndex)
            {
                return *this;
            }

            return path.SubStr(lastSeparatorIndex + separatorLen);
        }

        template<typename TChar>
        template<typename T>
        constexpr String_Base<T> TPath<TChar>::GetString() const noexcept
        {
            if constexpr (AreSameType<T, TChar>)
            {
                return m_path;
            }
            else if constexpr (AreSameType<T, char>)
            {
                return WStrToStr(GetTString());
            }
            else if constexpr (AreSameType<T, wchar_t>)
            {
                return StrToWStr(GetTString());
            }
            else
            {
                JPT_ASSERT(false, "Unsupported type for TPath::ConstBuffer");
                return nullptr;
            }
        }

        template<typename TChar>
        constexpr const typename TPath<TChar>::TString& TPath<TChar>::GetTString() const noexcept
        {
            return m_path;
        }

        template<typename TChar>
        constexpr const TChar* TPath<TChar>::ConstBuffer() const noexcept
        {
            return m_path.ConstBuffer();
        }

        template<typename TChar>
        constexpr size_t TPath<TChar>::Count() const
        {
            return m_path.Count();
        }

        template<typename TChar>
        constexpr bool TPath<TChar>::IsEmpty() const
        {
            return m_path.IsEmpty();
        }

        template<typename TChar>
        constexpr void TPath<TChar>::Reserve(size_t size)
        {
            m_path.Reserve(size);
        }

        template<typename TChar>
        constexpr void TPath<TChar>::Append(const TPath& path)
        {
            m_path.Append(path.ConstBuffer());
            FixSeparators(m_path);
        }

        template<typename TChar>
        constexpr void TPath<TChar>::operator+=(const WString& path)
        {
            m_path += path;
            FixSeparators(m_path);
        }

        template<typename TChar>
        constexpr TPath<TChar>& TPath<TChar>::Replace(const TChar* StringToFind, const TChar* StringToReplace, size_t startIndex, size_t endIndex)
        {
            m_path.Replace(StringToFind, StringToReplace, startIndex, endIndex);
            return *this;
        }

        template<typename TChar>
        constexpr bool TPath<TChar>::Has(const TPath& path) const
        {
            return m_path.Has(path.ConstBuffer());
        }

        template<typename TChar>
        constexpr bool TPath<TChar>::EndsWith(const TPath& path) const
        {
            return m_path.EndsWith(path.ConstBuffer());
        }
    }

    // ------------------------------------------------------------------------------------------------
    // Non member functions
    // ------------------------------------------------------------------------------------------------
    String ToString(const File::Path& path)
    {
        return path.GetString<char>();
    }
}