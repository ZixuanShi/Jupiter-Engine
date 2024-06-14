// Copyright Jupiter Technologies, Inc. All Rights Reserved.

module;

#include "Debugging/Assert.h"
#include "Debugging/Logger.h"

#include <fstream>
#include <string>

export module jpt.File_Text;

import jpt.String;

import jpt.File_Base;
import jpt.File.Path;
import jpt.File.PathUtils;
import jpt.File.Enums;

export namespace jpt::File
{
	/** Text file abstraction. */
	class File_Text : public File_Base
	{
	private:
		WString m_text;

	public:
		constexpr File_Text() = default;
		constexpr File_Text(const Path& path);

		static File_Text* Load(const Path& absoluteFullPath);

		void Save(const Path& absoluteFullPath) const;

		WString& GetText() { return m_text; }
		const WString& GetText() const { return m_text; }
	};

	constexpr File_Text::File_Text(const Path& path)
		: File_Base(path)
	{
	}

	File_Text* File_Text::Load(const Path& absoluteFullPath)
	{
		using TChar = typename Path::TChar;

		std::basic_ifstream<TChar> file;
		file.open(absoluteFullPath.ConstBuffer());

		if (!file.is_open())
		{
			JPT_ERROR("Failed to open file: %s", absoluteFullPath.ConstBuffer());
			return nullptr;
		}

		std::basic_string<TChar> line;
		File_Text* fileText = new File_Text(absoluteFullPath);
		while (std::getline(file, line))
		{
			fileText->m_text += line.c_str();
			fileText->m_text += '\n';
		}

		file.close();
		return fileText;
	}

	void File_Text::Save(const Path& absoluteFullPath) const
	{
		using TChar = typename Path::TChar;

		std::basic_ofstream<TChar> file;
		file.open(absoluteFullPath.ConstBuffer());

		if (!file.is_open())
		{
			JPT_ERROR("Failed to open file: %s", absoluteFullPath.ConstBuffer());
			return;
		}

		file << m_text.ConstBuffer();
		file.close();
	}
}