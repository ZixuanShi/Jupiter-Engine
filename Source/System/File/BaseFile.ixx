// Copyright Jupiter Technologies, Inc. All Rights Reserved.

module;

export module jpt.BaseFile;

import jpt.FileEnums;
import jpt.FileTypeDefs;
import jpt.FileUtils;
import jpt.TypeDefs;
import jpt.String;

export namespace jpt
{
	class BaseFile
	{
	private:
		TFilePath m_fileName;
		TFilePath m_filePath;
		EFileType m_fileType;

	public:
		virtual ~BaseFile() = default;
	};
}