// Copyright Jupiter Technologies, Inc. All Rights Reserved.

module;

#include <filesystem>

export module jpt.FilePath;

import jpt.Constants;
import jpt.String;
import jpt.StringUtils;
import jpt.ToString;
import jpt.Utilities;

export namespace jpt
{
	/** Identifies address of a file */
	class FilePath
	{
	public:
		using TChar = wchar_t;
		using TData = TChar;
		using TString = WString;

	private:
		TString m_path;

	public:
		constexpr FilePath() = default;
		constexpr FilePath(const char* path);
		constexpr FilePath(const wchar_t* path);
		constexpr FilePath(const TString& path);

		constexpr void Append(const FilePath& path);
		constexpr void operator+=(const WString& path);

		constexpr FilePath& Replace(const TChar* StringToFind, const TChar* StringToReplace, size_t startIndex = 0, size_t endIndex = npos);
		constexpr FilePath GetReplaced(const TChar* StringToFind, const TChar* StringToReplace, size_t startIndex = 0, size_t endIndex = npos) const;

		constexpr size_t Find(      wchar_t  charToFind,    size_t startIndex = 0, size_t endIndex = npos, size_t count = 1) const;
		constexpr size_t Find(const wchar_t* pStringToFind, size_t startIndex = 0, size_t endIndex = npos, size_t count = 1) const;
		constexpr size_t FindFirstOf(      wchar_t charToFind,     size_t startIndex = 0, size_t endIndex = npos, size_t count = 1) const;
		constexpr size_t FindFirstOf(const wchar_t* pStringToFind, size_t startIndex = 0, size_t endIndex = npos, size_t count = 1) const;
		constexpr size_t FindLastOf(      wchar_t charToFind,     size_t startIndex = 0, size_t endIndex = npos, size_t count = 1)  const;
		constexpr size_t FindLastOf(const wchar_t* pStringToFind, size_t startIndex = 0, size_t endIndex = npos, size_t count = 1)  const;
		constexpr bool   Contains(      wchar_t  charToFind,    size_t startIndex = 0, size_t endIndex = npos, size_t count = 1) const { return Find(charToFind, startIndex, endIndex, count)    != npos; }
		constexpr bool   Contains(const wchar_t* pStringToFind, size_t startIndex = 0, size_t endIndex = npos, size_t count = 1) const { return Find(pStringToFind, startIndex, endIndex, count) != npos; }

		constexpr const WString& ToWString() const { return m_path; }
		constexpr const wchar_t* ConstBuffer() const { return m_path.ConstBuffer(); }
		constexpr size_t Count() const { return m_path.Count(); }
	};

	// Non member functions -------------------------------------------------------------------------------------------------------------------

	constexpr bool operator==(const FilePath& filePath, const typename FilePath::TChar* CString)
	{
		return filePath.ToWString() == CString;
	}
	constexpr bool operator==(const FilePath& lhs, const FilePath& rhs)
	{
		return lhs.ToWString() == rhs.ToWString();
	}

	// Member Functions Definitions ---------------------------------------------------------------------------------------

	constexpr FilePath::FilePath(const char* path)
		: m_path(Move(jpt::ToWString(path)))
	{
	}

	constexpr FilePath::FilePath(const wchar_t* path)
		: m_path(path)
	{
	}

	constexpr FilePath::FilePath(const TString& path)
		: m_path(path)
	{
	}

	constexpr void FilePath::Append(const FilePath& path)
	{
		m_path.Append(path.ToWString());
	}

	constexpr void FilePath::operator+=(const WString& path)
	{
		m_path += path;
	}

	constexpr FilePath& FilePath::Replace(const TChar* StringToFind, const TChar* StringToReplace, size_t startIndex, size_t endIndex)
	{
		m_path.Replace(StringToFind, StringToReplace, startIndex, endIndex);
		return *this;
	}

	constexpr FilePath FilePath::GetReplaced(const TChar* StringToFind, const TChar* StringToReplace, size_t startIndex, size_t endIndex) const
	{
		return m_path.GetReplaced(StringToFind, StringToReplace, startIndex, endIndex);
	}

	constexpr size_t FilePath::Find(wchar_t charToFind, size_t startIndex /* = 0*/, size_t endIndex /* = npos*/, size_t count/* = 1*/) const
	{
		return m_path.Find(charToFind, startIndex, endIndex, count);
	}

	constexpr size_t FilePath::Find(const wchar_t* pStringToFind, size_t startIndex /* = 0*/, size_t endIndex /* = npos*/, size_t count/* = 1*/) const
	{
		return m_path.Find(pStringToFind, startIndex, endIndex, count);
	}

	constexpr size_t FilePath::FindFirstOf(wchar_t charToFind, size_t startIndex /* = 0*/, size_t endIndex /* = npos*/, size_t count/* = 1*/) const
	{
		return Find(charToFind, startIndex, endIndex, count);
	}

	constexpr size_t FilePath::FindFirstOf(const wchar_t* pStringToFind, size_t startIndex /* = 0*/, size_t endIndex /* = npos*/, size_t count/* = 1*/) const
	{
		return Find(pStringToFind, startIndex, endIndex, count);
	}

	constexpr size_t FilePath::FindLastOf(wchar_t charToFind, size_t startIndex, size_t endIndex, size_t count) const
	{
		return m_path.FindLastOf(charToFind, startIndex, endIndex, count);
	}

	constexpr size_t FilePath::FindLastOf(const wchar_t* pStringToFind, size_t startIndex, size_t endIndex, size_t count) const
	{
		return m_path.FindLastOf(pStringToFind, startIndex, endIndex, count);
	}
}