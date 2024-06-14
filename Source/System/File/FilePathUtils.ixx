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
	export namespace File
	{
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