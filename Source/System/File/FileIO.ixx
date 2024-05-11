// Copyright Jupiter Technologies, Inc. All Rights Reserved.

module;

#include "Core/Minimal/Macros.h"
#include "Debugging/Logger.h"
#include "Debugging/Assert.h"

#include <fstream>
#include <mutex>
#include <string>

export module jpt.FileIO;

import jpt.File_Base;
import jpt.FileEnums;
import jpt.FileIOUtils;
import jpt.FilePath;
import jpt.FilePathUtils;
import jpt.TypeDefs;

export namespace jpt
{
	File_Base* ReadFile(const FilePath& absoluteFullPath)
	{
		// Get extension to determine file type
		//EExtension extension = GetExtension(absoluteFullPath);

		//switch (extension)
		//{
		//	case EExtension::txt:
		//	case EExtension::json:
		//	{
		//		return ReadTextFile(absoluteFullPath);
		//	}
		//}

		JPT_IGNORE(absoluteFullPath);
		return nullptr;
	}

	template<typename TFile>
	TFile* ReadFile(ESource source, const FilePath& relativePath)
	{
		const FilePath path = GetAbsoluteFullPath(source, relativePath);
		File_Base* pFile = ReadFile(path);
		return static_cast<TFile*>(pFile);
	}
}