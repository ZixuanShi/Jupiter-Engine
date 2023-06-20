// Copyright Jupiter Technologies, Inc. All Rights Reserved.

#pragma once

namespace jpt
{
	/** Abstracted file base class */
	class File
	{
	protected:
		jpt::string m_folderPath;	/**< Abosulte path to this file's located folder */
		jpt::string m_name;			/**< The name of this file, excluding extension */
		jpt::string m_extension;	/**< The extension representing the type of this file */
		size_t m_sizeInMB = kInvalidValue<size_t>;	/**< How many mb this file takes. kInvalidValue if we don't know it's size */

	public:
		File() = default;
		File(const char* pFullAbsolutePath, size_t sizeInMB = kInvalidValue<size_t>);
		File(const char* pFolderPath, const char* pName, const char* pExtension, size_t sizeInMB = kInvalidValue<size_t>);
	};
}

