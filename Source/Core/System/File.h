// Copyright Jupiter Technologies, Inc. All Rights Reserved.

#pragma once

namespace jpt
{
	/** Abstracted file base class */
	class File
	{
	protected:
		jpt::string m_path;	/**< Abosulte path to this file. Including folder, name, and extension. Empty if we don't know it's path */
		size_t m_sizeInMB = kInvalidValue<size_t>;	/**< How many mb this file takes. kInvalidValue if we don't know it's size */

	public:
		File() = default;
		File(const char* pFullAbsolutePath, size_t sizeInMB = kInvalidValue<size_t>);

		/**< Returns a string of full absolute path */
		const jpt::string& GetPath() const { return m_path; }

		/** @return Abosulte path to this file's located folder */
		jpt::string_view GetFolderPath() const;

		/** @return The name of this file, excluding extension */
		jpt::string_view GetName() const;

		/** @return The extension representing the type of this file */
		jpt::string_view GetExtension() const;

		size_t GetSizeInMB() const { return m_sizeInMB; }

	private:
		size_t m_lastSlashIndex = jpt::string::npos;	/**< Last presented slash in the path */
		size_t m_dotIndex       = jpt::string::npos;	/**< Index of the dot in the path */
	};
}

