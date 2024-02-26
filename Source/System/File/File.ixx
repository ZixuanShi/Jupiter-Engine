// Copyright Jupiter Technologies, Inc. All Rights Reserved.

module;

export module jpt.File;

import jpt.String;

export namespace jpt
{
	class File
	{
	private:
		String m_content;
		String m_path;
		size_t m_size;
	};
}