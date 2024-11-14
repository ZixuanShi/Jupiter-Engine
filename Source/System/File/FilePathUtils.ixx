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
	/** @return Either full absolute path if ran from VS debugger, or relative path if ran from executable
		@param relativePath		Expected in client's folder. Not engine */
	Path FixDependencies(const Path& relativePath)
	{
#if IS_RELEASE
		// If ran from VS debugger, use absolute path to Client as it's using Visual Studio's working directory
		if (IsDebuggerPresent())
		{
			if (relativePath.Has("Jupiter_Common"))
			{
				return Path(Source::Engine, relativePath);
			}
			else
			{
				return Path(Source::Client, relativePath);
			}
		}
		// Release config will have copied assets to the output directory
		else
		{
			return relativePath;
		}
#else
		if (relativePath.Has("Jupiter_Common"))
		{
			return Path(Source::Engine, relativePath);
		}
		else
		{
			return Path(Source::Client, relativePath);
		}
#endif
	}
}