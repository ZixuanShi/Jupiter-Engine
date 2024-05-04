// Copyright Jupiter Technologies, Inc. All Rights Reserved.

module;

export module jpt.BaseFile;

import jpt.File.Enums;
import jpt.File.Path;
import jpt.File.Utils;
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