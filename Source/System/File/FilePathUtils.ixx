// Copyright Jupiter Technologies, Inc. All Rights Reserved.

module;

#include "Debugging/Assert.h"

export module jpt.File.Path.Utils;

import jpt.Environment;
import jpt.File.Enums;
import jpt.File.Path;

export namespace jpt::File
{
	constexpr const wchar_t* GetClientDirW();
	constexpr const wchar_t* GetOutputDirW();

	const Path EngineDir = JPT_ENGINE_DIR_W;
	const Path ClientDir = GetClientDirW();
	const Path OutputDir = GetOutputDirW();
}

export namespace jpt::File
{
	constexpr Path GetSourcePath(ESource source)
	{
		switch (source)
		{
			case ESource::Engine: return EngineDir;
			case ESource::Client: return ClientDir;
			case ESource::Output: return OutputDir;
			default: JPT_ASSERT(false, "Invalid source"); return Path();
		}
	}

	/** @return		The absolute full path of the given relative path */
	constexpr Path GetAbsoluteFullPath(ESource source, const Path& relativePath)
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
		// If ran from VS debugger, use the project's root directory
		if (IsDebuggerPresent())
		{
			return GetAbsoluteFullPath(ESource::Client, relativePath);
		}

		// If ran from the executable
#if IS_RELEASE
		// Release config will have copied assets to the output directory
		return relativePath;
#else
		// Non-Release config will use the client's directory
		return GetAbsoluteFullPath(ESource::Client, relativePath);
#endif
	}
}