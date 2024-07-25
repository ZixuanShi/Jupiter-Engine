// Copyright Jupiter Technologies, Inc. All Rights Reserved.

module;

#include "Debugging/Assert.h"

export module jpt.File.Path;

import jpt.String;
import jpt.ToString;
import jpt.Utilities;
import jpt.File.Enums;
export import jpt.File.Path.Helpers;

// TODO: Remove this when we have a better solution
export namespace jpt
{
	/** Designed to be implemented in client project's application
		@return Client's project root directory. */
	const char* GetClientDir();
	const wchar_t* GetClientDirW();
}

export namespace jpt::File
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

		constexpr void Append(const Path& path);
		constexpr void operator+=(const WString& path);

		constexpr Path& Replace(const TChar* StringToFind, const TChar* StringToReplace, size_t startIndex = 0, size_t endIndex = npos);
		constexpr Path GetReplaced(const TChar* StringToFind, const TChar* StringToReplace, size_t startIndex = 0, size_t endIndex = npos) const;

		constexpr size_t FindLastOf(const Path& path) const;
		constexpr bool Has(const Path& path) const;

		constexpr const WString& ToWString() const { return m_path; }
		constexpr const wchar_t* ConstBuffer() const { return m_path.ConstBuffer(); }
		constexpr size_t Count() const { return m_path.Count(); }
	};

	// Non member functions -----------------------------------------------------------------------------------------------

	constexpr bool operator==(const Path& filePath, const typename Path::TChar* CString)
	{
		return filePath.ToWString() == CString;
	}
	constexpr bool operator==(const Path& lhs, const Path& rhs)
	{
		return lhs.ToWString() == rhs.ToWString();
	}

	// Member Functions Definitions ---------------------------------------------------------------------------------------

	constexpr Path::Path(char path)
		: m_path(Move(jpt::ToWString(jpt::String(path))))
	{
	}

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

	constexpr Path Path::GetReplaced(const TChar* StringToFind, const TChar* StringToReplace, size_t startIndex, size_t endIndex) const
	{
		return m_path.GetReplaced(StringToFind, StringToReplace, startIndex, endIndex);
	}

	constexpr size_t Path::FindLastOf(const Path& path) const
	{
		return m_path.FindLastOf(path.ToWString().ConstBuffer());
	}

	constexpr bool Path::Has(const Path& path) const
	{
		return m_path.Has(path.ToWString().ConstBuffer());
	}
}