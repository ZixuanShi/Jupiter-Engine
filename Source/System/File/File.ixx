// Copyright Jupiter Technologies, Inc. All Rights Reserved.

module;

#include "Debugging/Assert.h"
#include "Debugging/Logger.h"

#include <fstream>

export module jpt.File;

import jpt.Any;
import jpt.Concepts;
import jpt.Optional;
import jpt.Serializer;
import jpt.Utilities;
import jpt.File.Path;

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
		template<StringType TString> bool LoadText(const Path& absoluteFullPath);
		template<StringType TString> bool SaveText(const Path& absoluteFullPath);

		// Binary
		template<typename T> bool LoadBinary(const Path& absoluteFullPath);
		template<typename T> bool SaveBinary(const Path& absoluteFullPath);

		// Data
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

	template<StringType TString>
	bool File::LoadText(const Path& absoluteFullPath)
	{
		m_path = absoluteFullPath;

		if (Optional<TString> content = LoadTextFile<TString>(absoluteFullPath))
		{
			m_data = content.Value();
			return true;
		}

		return false;
	}

	template<StringType TString>
	bool File::SaveText(const Path& absoluteFullPath)
	{
		const TString& content = m_data.As<TString>();
		return SaveTextFile<TString::TChar>(absoluteFullPath, content.ConstBuffer(), content.Size());
	}

	template<typename T>
	bool File::LoadBinary(const Path& absoluteFullPath)
	{
		m_path = absoluteFullPath;
		m_data = T();
		return LoadBinaryFile(absoluteFullPath, m_data.As<T>());
	}

	template<typename T>
	bool File::SaveBinary(const Path& absoluteFullPath)
	{
		return SaveBinaryFile(absoluteFullPath, m_data.As<T>());
	}
}