// Copyright Jupiter Technologies, Inc. All Rights Reserved.

module;

export module jpt.File_Base;

import jpt.FileEnums;
import jpt.FilePath;
import jpt.FileUtils;
import jpt.TypeDefs;
import jpt.String;

export namespace jpt
{
	class File_Base
	{
	private:
		FilePath m_fileName;
		FilePath m_filePath;
		EExtension m_fileType;

	public:
		virtual ~File_Base() = default;


	};
}