// Copyright Jupiter Technologies, Inc. All Rights Reserved.

module; 

#include "Core/Minimal/Macros.h"
#include "Core/Types/Enum.h"
#include "Debugging/Assert.h"

#include <stdlib.h>

export module jpt.FileUtils;

import jpt.FileEnums;
import jpt.FilePath;
import jpt.FileTypeDefs;
import jpt.String;
import jpt.StringView;
import jpt.ToString;
import jpt.TypeTraits;
import jpt.Utilities;

export namespace jpt
{
	/** Designed to be implemented in client project's application
		@return Client's project root directory. */
	const char* GetClientDir();
	const wchar_t* GetClientDirW();

	/** @return		 FilePath type of engine/client directories roots */
	FilePath GetEnginePath()
	{
		using TChar = typename FilePath::TChar;

		return []()
			{
				if constexpr (AreSameType<TChar, char>)
				{
					return JPT_ENGINE_DIR;
				}
				else if constexpr (AreSameType<TChar, wchar_t>)
				{
					return JPT_ENGINE_DIR_W;
				}
			}();
	}
	FilePath GetClientPath()
	{
		using TChar = typename FilePath::TChar;

		return []()
			{
				if constexpr (AreSameType<TChar, char>)
				{
					return GetClientDir();
				}
				else if constexpr (AreSameType<TChar, wchar_t>)
				{
					return GetClientDirW();
				}
			}();
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