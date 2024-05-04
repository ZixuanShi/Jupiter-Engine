// Copyright Jupiter Technologies, Inc. All Rights Reserved.

module;

export module jpt.BaseFile;

import jpt.FileEnums;
import jpt.FilePath;
import jpt.FileTypeDefs;
import jpt.FileUtils;
import jpt.TypeDefs;
import jpt.String;

export namespace jpt
{
	class BaseFile
	{
	private:
		FilePath m_fileName;
		FilePath m_filePath;
		EExtension m_fileType;

	public:
		virtual ~BaseFile() = default;


	};
}