// Copyright Jupiter Technologies, Inc. All Rights Reserved.

module;

export module jpt.File_Base;

import jpt.TypeDefs;

import jpt.File.Enums;
import jpt.File.Path;

export namespace jpt::File
{
	/** Base file abstraction for all files. */
	class File_Base
	{
	protected:
		Path m_path;

	public:
		constexpr File_Base() = default;
		constexpr File_Base(const Path& path) : m_path(path) {}
		constexpr virtual ~File_Base() = default;

		constexpr const Path& GetPath() const { return m_path; }
	};
}