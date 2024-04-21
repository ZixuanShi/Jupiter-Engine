// Copyright Jupiter Technologies, Inc. All Rights Reserved.

module; 

#include "Core/Minimal/Macros.h"
#include "Core/Types/Enum.h"
#include "Debugging/Assert.h"

export module jpt.FileUtils;

import jpt.FileEnums;
import jpt.FileTypeDefs;
import jpt.String;
import jpt.StringView;
import jpt.TypeTraits;

export namespace jpt
{
	/** Designed to be implemented in client project's application
		@return Client's project root directory. */
	const char* GetClientDir();
	const wchar_t* GetClientDirW();

	/** Replaces directory slashes to platform-correct version */
	template<StringType TString>
	void FixSlashes(TString& path)
	{
		using TChar = typename TString::TChar;

		path.Replace(JPT_GET_PROPER_STRING(TChar, \\), JPT_GET_PROPER_STRING(TChar, / ));
	}

	/** Returns the absolute path of the given relative path */
	FilePath GetAbsolutePath(ESource source, FilePathView relativePath)
	{
		using TChar = typename FilePath::TChar;

		FilePath result;

		switch (source)
		{
		case ESource::Engine:
		{
			result.Append([]()
				{
					if constexpr (AreSameType<TChar, char>)
					{
						return JPT_ENGINE_DIR;
					}
					else if constexpr (AreSameType<TChar, wchar_t>)
					{
						return JPT_ENGINE_DIR_W;
					}
				}());

			break;
		}
		case ESource::Client:
		{
			result.Append([]()
				{
					if constexpr (AreSameType<TChar, char>)
					{
						return GetClientDir();
					}
					else if constexpr (AreSameType<TChar, wchar_t>)
					{
						return GetClientDirW();
					}
				}());
			
			break;
		}

		default:
			JPT_ASSERT(false, "Invalid source");
			break;
		}

		result.Append(relativePath.ConstBuffer(), relativePath.Count());
		return result;
	}
}