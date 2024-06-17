// Copyright Jupiter Technologies, Inc. All Rights Reserved.

module;

#include "Debugging/Assert.h"
#include "Debugging/Logger.h"

#include <fstream>

export module jpt.File;

import jpt.Any;
import jpt.Concepts;
import jpt.Optional;
import jpt.SerializationUtils;
import jpt.Utilities;

import jpt.File.Path;
import jpt.FileIO;

export namespace jpt::File
{
	/** Represents a File in the system
		Could contain text, binary, or any custom data */
	class File
	{
	private:
		Path m_path;	/**< Absolute full path to this file */
		Any m_data;		/**< Data of this file. Could be WString for text, Custom type, etc. */

	public:
		constexpr File() = default;
		constexpr File(const Path& path);

		// Text
		bool LoadText(const Path& absoluteFullPath);
		void SaveText(const Path& absoluteFullPath);

		// Binary
		template<typename T> void LoadBinary(const Path& absoluteFullPath);
		template<typename T> void SaveBinary(const Path& absoluteFullPath);

		// Data
		void SetText(const String& text) { m_data = text; }
		const String& GetText() const { return m_data.As<String>(); }
		template<typename T> void SetData(const T& data) { m_data = data; }
		template<typename T>       T& GetData()       { return m_data.As<T>(); }
		template<typename T> const T& GetData() const { return m_data.As<T>(); }

		// Path
		const Path& GetPath() const { return m_path; }
	};

	constexpr File::File(const Path& path)
		: m_path(path)
	{
	}

	bool File::LoadText(const Path& absoluteFullPath)
	{
		m_path = absoluteFullPath;

		if (Optional<jpt::String> content = LoadTextFile(absoluteFullPath))
		{
			m_data = content.Value();
			return true;
		}

		return false;
	}

	void File::SaveText(const Path& absoluteFullPath)
	{
		const String& content = m_data.As<String>();
		SaveTextFile(absoluteFullPath, content.ConstBuffer(), content.Size());
	}

	template<typename T>
	void File::LoadBinary(const Path& absoluteFullPath)
	{
		m_path = absoluteFullPath;
		m_data = T();
		LoadBinaryFile(absoluteFullPath, m_data.As<T>());
	}

	template<typename T>
	void File::SaveBinary(const Path& absoluteFullPath)
	{
		SaveBinaryFile(absoluteFullPath, m_data.As<T>());
	}
}