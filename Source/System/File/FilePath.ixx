// Copyright Jupiter Technologies, Inc. All Rights Reserved.

module;

#include "Core/Minimal/Macros.h"
#include "Debugging/Assert.h"
#include "Debugging/Logger.h"

#include <filesystem>

export module jpt.File.Path;

import jpt.Concepts;
import jpt.Constants;
import jpt.String;
import jpt.StringUtils;
import jpt.ToString;
import jpt.TypeTraits;
import jpt.Utilities;

export namespace jpt::File
{
	/** Replaces directory slashes to platform-correct version */
	template<typename TString>
	constexpr void FixSeparators(TString& path)
	{
		using TChar = typename TString::TChar;

#if IS_PLATFORM_WIN64 || IS_PLATFORM_XBOX
		path.Replace(JPT_GET_PROPER_STRING(TChar, / ), JPT_GET_PROPER_STRING(TChar, \\));
#else
		path.Replace(JPT_GET_PROPER_STRING(TChar, \\), JPT_GET_PROPER_STRING(TChar, / ));
#endif
	}

	template<typename TString>
	constexpr TString GetSeparator()
	{
		using TChar = typename TString::TChar;

#if IS_PLATFORM_WIN64 || IS_PLATFORM_XBOX
		return JPT_GET_PROPER_STRING(TChar, \\);
#else
		return JPT_GET_PROPER_STRING(TChar, / );
#endif
	}

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
		constexpr Path(const char* path);
		constexpr Path(const wchar_t* path);
		constexpr Path(const TString& path);

		constexpr void Append(const Path& path);
		constexpr void operator+=(const WString& path);

		constexpr Path& Replace(const TChar* StringToFind, const TChar* StringToReplace, size_t startIndex = 0, size_t endIndex = npos);
		constexpr Path GetReplaced(const TChar* StringToFind, const TChar* StringToReplace, size_t startIndex = 0, size_t endIndex = npos) const;

		constexpr size_t Find(      wchar_t  charToFind,    size_t startIndex = 0, size_t endIndex = npos, size_t count = 1) const;
		constexpr size_t Find(const wchar_t* pStringToFind, size_t startIndex = 0, size_t endIndex = npos, size_t count = 1) const;
		constexpr size_t FindFirstOf(      wchar_t charToFind,     size_t startIndex = 0, size_t endIndex = npos, size_t count = 1) const;
		constexpr size_t FindFirstOf(const wchar_t* pStringToFind, size_t startIndex = 0, size_t endIndex = npos, size_t count = 1) const;
		constexpr size_t FindLastOf(      wchar_t charToFind,     size_t startIndex = 0, size_t endIndex = npos, size_t count = 1)  const;
		constexpr size_t FindLastOf(const wchar_t* pStringToFind, size_t startIndex = 0, size_t endIndex = npos, size_t count = 1)  const;
		constexpr bool   Contains(      wchar_t  charToFind,    size_t startIndex = 0, size_t endIndex = npos, size_t count = 1) const { return Find(charToFind, startIndex, endIndex, count)    != npos; }
		constexpr bool   Contains(const wchar_t* pStringToFind, size_t startIndex = 0, size_t endIndex = npos, size_t count = 1) const { return Find(pStringToFind, startIndex, endIndex, count) != npos; }

		constexpr const WString& ToWString() const { return m_path; }
		constexpr const WString& ToString() const { return m_path; }
		constexpr const wchar_t* ConstBuffer() const { return m_path.ConstBuffer(); }
		constexpr size_t Count() const { return m_path.Count(); }
	};

	// Non member functions -------------------------------------------------------------------------------------------------------------------

	constexpr bool operator==(const Path& filePath, const typename Path::TChar* CString)
	{
		return filePath.ToWString() == CString;
	}
	constexpr bool operator==(const Path& lhs, const Path& rhs)
	{
		return lhs.ToWString() == rhs.ToWString();
	}

	// Member Functions Definitions ---------------------------------------------------------------------------------------

	constexpr Path::Path(const char* path)
		: m_path(Move(jpt::ToWString(path)))
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

	constexpr Path Path::GetReplaced(const TChar* StringToFind, const TChar* StringToReplace, size_t startIndex, size_t endIndex) const
	{
		return m_path.GetReplaced(StringToFind, StringToReplace, startIndex, endIndex);
	}

	constexpr size_t Path::Find(wchar_t charToFind, size_t startIndex /* = 0*/, size_t endIndex /* = npos*/, size_t count/* = 1*/) const
	{
		return m_path.Find(charToFind, startIndex, endIndex, count);
	}

	constexpr size_t Path::Find(const wchar_t* pStringToFind, size_t startIndex /* = 0*/, size_t endIndex /* = npos*/, size_t count/* = 1*/) const
	{
		return m_path.Find(pStringToFind, startIndex, endIndex, count);
	}

	constexpr size_t Path::FindFirstOf(wchar_t charToFind, size_t startIndex /* = 0*/, size_t endIndex /* = npos*/, size_t count/* = 1*/) const
	{
		return Find(charToFind, startIndex, endIndex, count);
	}

	constexpr size_t Path::FindFirstOf(const wchar_t* pStringToFind, size_t startIndex /* = 0*/, size_t endIndex /* = npos*/, size_t count/* = 1*/) const
	{
		return Find(pStringToFind, startIndex, endIndex, count);
	}

	constexpr size_t Path::FindLastOf(wchar_t charToFind, size_t startIndex, size_t endIndex, size_t count) const
	{
		return m_path.FindLastOf(charToFind, startIndex, endIndex, count);
	}

	constexpr size_t Path::FindLastOf(const wchar_t* pStringToFind, size_t startIndex, size_t endIndex, size_t count) const
	{
		return m_path.FindLastOf(pStringToFind, startIndex, endIndex, count);
	}
}