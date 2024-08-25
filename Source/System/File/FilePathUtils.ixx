// Copyright Jupiter Technologies, Inc. All Rights Reserved.

module;

#include "Debugging/Assert.h"

export module jpt.File.Path.Utils;

import jpt.Environment;
import jpt.File.Enums;
import jpt.File.Path;
import jpt.System.Paths;

export namespace jpt::File
{
	constexpr Path GetSourcePath(Source source)
	{
		switch (source)
		{
			case Source::Engine: return System::Paths::GetInstance().GetEngineDir();
			case Source::Client: return System::Paths::GetInstance().GetClientDir();
			case Source::Output: return System::Paths::GetInstance().GetOutputDir();
			case Source::Saved:  return System::Paths::GetInstance().GetSavedDir();
			default: JPT_ASSERT(false, "Invalid source"); return Path();
		}
	}

	/** @return		The absolute full path of the given relative path */
	constexpr Path GetAbsoluteFullPath(Source source, const Path& relativePath)
	{
		Path result;

		result.Append(GetSourcePath(source));
		result.Append(relativePath);

		return result;
	}

	/** @return Either full absolute path if ran from VS debugger, or relative path if ran from executable
		@param relativePath		Expected in client's folder. Not engine */
	Path FixDependencies(const Path& relativePath)
	{
#if IS_RELEASE
		// Release config will have copied assets to the output directory
		return relativePath;
#else
		if (relativePath.Has("Jupiter_Common"))
		{
			return GetAbsoluteFullPath(Source::Engine, relativePath);
		}
		else
		{
			return GetAbsoluteFullPath(Source::Client, relativePath);
		}
#endif
	}
}