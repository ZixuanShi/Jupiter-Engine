// Copyright Jupiter Technologies, Inc. All Rights Reserved.

module;

#include "Core/Minimal/CoreMacros.h"
#include "Core/Validation/Assert.h"

export module jpt.FilePath;

import jpt.String;
import jpt.ToString;
import jpt.TypeDefs;
import jpt.Utilities;

import jpt.File_Enums;
import jpt.FilePathHelpers;

export namespace jpt
{
    namespace File
    {
        /** Identifies address of a file */
        class Path
        {
        public:
            using TChar = wchar_t;
            using TData = TChar;
            using TString = WString;

        private:
            TString m_path;

        public:
            constexpr Path() = default;
            constexpr Path(char path);
            constexpr Path(const char* path);
            constexpr Path(const wchar_t* path);
            constexpr Path(const TString& path);

        public:
            constexpr void Append(const Path& path);
            constexpr void operator+=(const WString& path);

            constexpr Path& Replace(const TChar* StringToFind, const TChar* StringToReplace, size_t startIndex = 0, size_t endIndex = npos);

            constexpr void Reserve(size_t size);

            constexpr size_t FindLastOf(const Path& path) const;
            constexpr bool Has(const Path& path) const;
            constexpr bool EndsWith(const Path& path) const;
            constexpr Path GetParent() const;
            constexpr Path SubPath(size_t index, size_t count = npos) const;

        public:
            constexpr const WString& ToWString() const { return m_path; }
            constexpr const wchar_t* ConstBuffer() const { return m_path.ConstBuffer(); }
            constexpr size_t Count() const { return m_path.Count(); }
            constexpr bool IsEmpty() const { return m_path.IsEmpty(); }
        };

        // ------------------------------------------------------------------------------------------------
        // Non member functions
        // ------------------------------------------------------------------------------------------------
        constexpr bool operator==(const Path& filePath, const typename Path::TChar* CString)
        {
            return filePath.ToWString() == CString;
        }
        constexpr bool operator==(const Path& lhs, const Path& rhs)
        {
            return lhs.ToWString() == rhs.ToWString();
        }

        constexpr Path operator+(const Path& lhs, const Path& rhs)
        {
            Path result = lhs;
            result.Append(rhs);
            return result;
        }

        constexpr uint64 Hash(const Path& path)
        {
            return Hash(path.ToWString());
        }

        String ToString(const Path& path)
        {
            return WStrToStr(path.ToWString());
        }

        // ------------------------------------------------------------------------------------------------
        // Member Functions
        // ------------------------------------------------------------------------------------------------
        constexpr Path::Path(char path)
            : m_path(Move(jpt::StrToWStr(jpt::String(path))))
        {
        }

        constexpr Path::Path(const char* path)
            : m_path(Move(jpt::CStrToWStr(path)))
        {
            FixSeparators(m_path);
        }

        constexpr Path::Path(const wchar_t* path)
            : m_path(path)
        {
            FixSeparators(m_path);
        }

        constexpr Path::Path(const TString& path)
            : m_path(path)
        {
            FixSeparators(m_path);
        }

        constexpr void Path::Append(const Path& path)
        {
            m_path.Append(path.ToWString());
            FixSeparators(m_path);
        }

        constexpr void Path::operator+=(const WString& path)
        {
            m_path += path;
            FixSeparators(m_path);
        }

        constexpr Path& Path::Replace(const TChar* StringToFind, const TChar* StringToReplace, size_t startIndex, size_t endIndex)
        {
            m_path.Replace(StringToFind, StringToReplace, startIndex, endIndex);
            return *this;
        }

        constexpr void Path::Reserve(size_t size)
        {
            m_path.Reserve(size);
        }

        constexpr size_t Path::FindLastOf(const Path& path) const
        {
            return m_path.FindLastOf(path.ToWString().ConstBuffer());
        }

        constexpr bool Path::Has(const Path& path) const
        {
            return m_path.Has(path.ToWString().ConstBuffer());
        }

        constexpr bool Path::EndsWith(const Path& path) const
        {
            return m_path.EndsWith(path.ToWString().ConstBuffer());
        }

        constexpr Path Path::GetParent() const
        {
            const TString seprator = GetSeparator<TString>();
            const size_t lastSeparatorIndex = m_path.FindLastOf(seprator.ConstBuffer());
            JPT_ASSERT(lastSeparatorIndex != npos, "Path has no parent");

            return m_path.SubStr(0, lastSeparatorIndex + seprator.Count());
        }

        constexpr Path Path::SubPath(size_t index, size_t count) const
        {
            return m_path.SubStr(index, count);
        }
    }
}