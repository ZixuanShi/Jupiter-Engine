// Copyright Jupiter Technologies, Inc. All Rights Reserved.

module;

#include "Debugging/Assert.h"
#include "Debugging/Logger.h"

#include <fstream>
#include <string>

export module jpt.File_Text;

import jpt.String;

import jpt.File_Base;

export namespace jpt::File
{
	/** Text file abstraction. */
	class File_Text : public File_Base
	{
	private:
		WString m_text;

	public:
		virtual bool Load(const Path& absoluteFullPath) override;

		const WString& GetText() const { return m_text; }
	};

	bool File_Text::Load(const Path& absoluteFullPath)
	{
		using TChar = typename Path::TChar;

		std::basic_ifstream<TChar> file;
		file.open(absoluteFullPath.ConstBuffer());

		if (!file.is_open())
		{
			JPT_ERROR("Failed to open file: %s", absoluteFullPath.ConstBuffer());
			return false;
		}

		std::basic_string<TChar> line;
		while (std::getline(file, line))
		{
			m_text += line.c_str();
			m_text += '\n';
		}

		file.close();
		return true;
	}
}