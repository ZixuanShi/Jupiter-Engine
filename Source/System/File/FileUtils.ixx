// Copyright Jupiter Technologies, Inc. All Rights Reserved.

module; 

#include "Core/Minimal/Macros.h"
#include "Core/Types/Enum.h"
#include "Debugging/Assert.h"

#include <stdlib.h>

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
	const char* GetClientDirImpl();
	const wchar_t* GetClientDirWImpl();

	/** @return		 FilePath type of engine/client directories roots */
	FilePath GetEngineDir()
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
	FilePath GetClientDir()
	{
		using TChar = typename FilePath::TChar;

		return []()
			{
				if constexpr (AreSameType<TChar, char>)
				{
					return GetClientDirImpl();
				}
				else if constexpr (AreSameType<TChar, wchar_t>)
				{
					return GetClientDirWImpl();
				}
			}();
	}

	/** Replaces directory slashes to platform-correct version */
	template<StringType TString>
	void FixSlashes(TString& path)
	{
		using TChar = typename TString::TChar;

		path.Replace(JPT_GET_PROPER_STRING(TChar, \\), JPT_GET_PROPER_STRING(TChar, / ));
	}

	/** @return		The absolute full path of the given relative path */
	FilePath GetAbsoluteFullPath(ESource source, FilePathView relativePath)
	{
		using TChar = typename FilePath::TChar;

		FilePath result;

		switch (source)
		{
		case ESource::Engine:
		{
			result.Append(GetEngineDir());
			break;
		}
		case ESource::Client:
		{
			result.Append(GetClientDir());			
			break;
		}

		default:
			JPT_ASSERT(false, "Invalid source");
			break;
		}

		result.Append(relativePath.ConstBuffer(), relativePath.Count());
		return result;
	}

	/** @return		FilePath's type of given pCStr */
	FilePath ToFilePathType(const char* pCStr)
	{
		using TChar = typename FilePath::TChar;

		return [pCStr]()
			{
				if constexpr (AreSameType<TChar, char>)
				{
					return pCStr;
				}
				else if constexpr (AreSameType<TChar, wchar_t>)
				{
					return ToWString(pCStr);
				}
			}();
	}
}