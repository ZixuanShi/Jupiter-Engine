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
	Path Combine(Source source, const Path& relativePath)
	{
		Path path;
		System::Paths& paths = System::Paths::GetInstance();

		switch (source)
		{
		case Source::Engine:
			path.Append(JPT_ENGINE_DIR_W);
			break;
		case Source::Client:
			path.Append(paths.GetClientDir());
			break;
		case Source::Output:
			path.Append(paths.GetOutputDir());
			break;
		case Source::Saved:
#if IS_RELEASE
			path.Append(paths.GetOutputDir());
#else
			path.Append(paths.GetClientDir());
#endif
			path.Append(L"_Saved/");
			break;

		default:
			JPT_ASSERT(false, "Unknown source");
		}

		path.Append(relativePath);

		return path;
	}

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
				return Combine(Source::Engine, relativePath);
			}
			else
			{
				return Combine(Source::Client, relativePath);
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
			return Combine(Source::Engine, relativePath);
		}
		else
		{
			return Combine(Source::Client, relativePath);
		}
#endif
	}
}