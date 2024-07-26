// Copyright Jupiter Technologies, Inc. All Rights Reserved.

module;

#include "Debugging/Assert.h"

export module jpt.File.Path.Utils;

import jpt.File.Enums;
import jpt.File.Path;
import jpt.SystemPaths;

export namespace jpt::File
{
	const Path EnginePath = JPT_ENGINE_DIR_W;
	const Path ClientPath = GetClientDirW();

	constexpr Path GetSourcePath(ESource source)
	{
		switch (source)
		{
			case ESource::Engine: return EnginePath;
			case ESource::Client: return ClientPath;
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

	// TODO: Implement this function
	constexpr Path ToCurrentRoot(const Path& relativePath)
	{
		// If ran from VS debugger, use the project's root directory


		// If ran from the executable, use the executable's directory

		return relativePath;
	}
}