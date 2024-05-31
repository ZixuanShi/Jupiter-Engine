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
	private:
		Path m_filePath;

	public:
		virtual ~File_Base() = default;


	};
}