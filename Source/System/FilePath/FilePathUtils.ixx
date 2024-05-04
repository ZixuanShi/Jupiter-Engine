// Copyright Jupiter Technologies, Inc. All Rights Reserved.

module;

#include "Core/Minimal/Macros.h"
#include "Debugging/Assert.h"

export module jpt.File.PathUtils;

import jpt.File.Enums;
import jpt.File.Path;
import jpt.String;

export namespace jpt
{
	/** Designed to be implemented in client project's application
		@return Client's project root directory. */
	const char* GetClientDir();
	const wchar_t* GetClientDirW();

	/** @return		 FilePath type of engine/client directories roots */
	FilePath GetEnginePath()
	{
		return JPT_ENGINE_DIR_W;
	}
	FilePath GetClientPath()
	{
		return GetClientDirW();
	}

	FilePath GetSourcePath(ESource source)
	{
		switch (source)
		{
			case ESource::Engine: return GetEnginePath();
			case ESource::Client: return GetClientPath();
			default: JPT_ASSERT(false, "Invalid source"); return FilePath();
		}
	}

	/** Replaces directory slashes to platform-correct version */
	template<StringType TString>
	void FixSlashes(TString& path)
	{
		using TChar = typename TString::TChar;

#if IS_PLATFORM_WIN64
		path.Replace(JPT_GET_PROPER_STRING(TChar, \\), JPT_GET_PROPER_STRING(TChar, / ));
#else
#error "Unsupported platform"
#endif
	}

	template<StringType TString>
	TString GetSlash()
	{
		using TChar = typename TString::TChar;

#if IS_PLATFORM_WIN64
		return JPT_GET_PROPER_STRING(TChar, /);
#else
	#error "Unsupported platform"
#endif
	}

	/** @return		The absolute full path of the given relative path */
	FilePath GetAbsoluteFullPath(ESource source, const FilePath& relativePath)
	{
		FilePath result;

		result.Append(GetSourcePath(source));
		result.Append(relativePath);

		return result;
	}
}