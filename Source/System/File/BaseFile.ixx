// Copyright Jupiter Technologies, Inc. All Rights Reserved.

module;

export module jpt.BaseFile;

import jpt.String;

export namespace jpt
{
	class BaseFile
	{
	private:
		String m_fileName;
		String m_filePath;

	public:
		virtual ~BaseFile() = default;
	};
}