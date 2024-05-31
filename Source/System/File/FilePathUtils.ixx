// Copyright Jupiter Technologies, Inc. All Rights Reserved.

module;

#include "Core/Minimal/Macros.h"
#include "Debugging/Assert.h"
#include "Debugging/Logger.h"

export module jpt.File.PathUtils;

import jpt.String;

import jpt.File.Enums;
import jpt.File.Path;

export namespace jpt
{
	/** Designed to be implemented in client project's application
		@return Client's project root directory. */
	const char* GetClientDir();
	const wchar_t* GetClientDirW();

	export namespace File
	{
		/** @return		 FilePath type of engine/client directories roots */
		Path GetEnginePath()
		{
			return JPT_ENGINE_DIR_W;
		}
		Path GetClientPath()
		{
			return GetClientDirW();
		}

		Path GetSourcePath(ESource source)
		{
			switch (source)
			{
			case ESource::Engine: return GetEnginePath();
			case ESource::Client: return GetClientPath();
			default: JPT_ASSERT(false, "Invalid source"); return Path();
			}
		}

		/** @return		The absolute full path of the given relative path */
		Path GetAbsoluteFullPath(ESource source, const Path& relativePath)
		{
			Path result;

			result.Append(GetSourcePath(source));
			result.Append(relativePath);

			return result;
		}

		/** @return		    File's extension of the given path
			@param path		Can be either relative or absolute. As long as it ends with .<type> format
			@example		GetExtension("Assets/Config/JupiterEngine.json") will return json */
		EExtension FindExtension(const Path& path)
		{
			const size_t dotPos = path.FindLastOf('.');
			if (dotPos == npos)
			{
				JPT_ERROR("Failed to find extension in path: %s", path.ConstBuffer());
				return EExtension::Unknown;
			}

			// TODO
			return EExtension::Unknown;
		}
	}
}