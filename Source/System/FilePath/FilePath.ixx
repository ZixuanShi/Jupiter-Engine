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
		using TString = WString;

	private:
		TString m_path;

	public:
		FilePath() = default;
		FilePath(const char* path);
		FilePath(const wchar_t* path);

		void Append(const FilePath& path);
		void operator+=(const WString& path);

		size_t Find(      wchar_t  charToFind,    size_t startIndex = 0, size_t endIndex = npos, size_t count = 1) const;
		size_t Find(const wchar_t* pStringToFind, size_t startIndex = 0, size_t endIndex = npos, size_t count = 1) const;
		size_t FindFirstOf(      wchar_t charToFind,     size_t startIndex = 0, size_t endIndex = npos, size_t count = 1) const;
		size_t FindFirstOf(const wchar_t* pStringToFind, size_t startIndex = 0, size_t endIndex = npos, size_t count = 1) const;
		size_t FindLastOf(      wchar_t charToFind,     size_t startIndex = 0, size_t endIndex = npos, size_t count = 1)  const;
		size_t FindLastOf(const wchar_t* pStringToFind, size_t startIndex = 0, size_t endIndex = npos, size_t count = 1)  const;
		bool   Contains(      wchar_t  charToFind,    size_t startIndex = 0, size_t endIndex = npos, size_t count = 1) const { return Find(charToFind, startIndex, endIndex, count)    != npos; }
		bool   Contains(const wchar_t* pStringToFind, size_t startIndex = 0, size_t endIndex = npos, size_t count = 1) const { return Find(pStringToFind, startIndex, endIndex, count) != npos; }

		const WString& ToWString() const { return m_path; }
		const wchar_t* ConstBuffer() const { return m_path.ConstBuffer(); }
		size_t Count() const { return m_path.Count(); }
	};

	FilePath::FilePath(const char* path)
		: m_path(Move(jpt::ToWString(path)))
	{
	}

	FilePath::FilePath(const wchar_t* path)
		: m_path(path)
	{
	}

	void FilePath::Append(const FilePath& path)
	{
		m_path.Append(path.ToWString());
	}

	void FilePath::operator+=(const WString& path)
	{
		m_path += path;
	}

	size_t FilePath::Find(wchar_t charToFind, size_t startIndex /* = 0*/, size_t endIndex /* = npos*/, size_t count/* = 1*/) const
	{
		return m_path.Find(charToFind, startIndex, endIndex, count);
	}

	size_t FilePath::Find(const wchar_t* pStringToFind, size_t startIndex /* = 0*/, size_t endIndex /* = npos*/, size_t count/* = 1*/) const
	{
		return m_path.Find(pStringToFind, startIndex, endIndex, count);
	}

	size_t FilePath::FindFirstOf(wchar_t charToFind, size_t startIndex /* = 0*/, size_t endIndex /* = npos*/, size_t count/* = 1*/) const
	{
		return Find(charToFind, startIndex, endIndex, count);
	}

	size_t FilePath::FindFirstOf(const wchar_t* pStringToFind, size_t startIndex /* = 0*/, size_t endIndex /* = npos*/, size_t count/* = 1*/) const
	{
		return Find(pStringToFind, startIndex, endIndex, count);
	}

	size_t FilePath::FindLastOf(wchar_t charToFind, size_t startIndex, size_t endIndex, size_t count) const
	{
		return m_path.FindLastOf(charToFind, startIndex, endIndex, count);
	}

	size_t FilePath::FindLastOf(const wchar_t* pStringToFind, size_t startIndex, size_t endIndex, size_t count) const
	{
		return m_path.FindLastOf(pStringToFind, startIndex, endIndex, count);
	}
}