// Copyright Jupiter Technologies, Inc. All Rights Reserved.

module;

#include "Core/Minimal/Headers.h"

export module UnitTests_System;

import jpt.Utilities;

/** Unit Test Modules */

// File
import UnitTests_FileUtils;
import UnitTests_FileIO;

// FileIO
import UnitTests_FileIO;

// FilePath
import UnitTests_FilePath;

// Time
import UnitTests_Clock;
import UnitTests_StopWatch;

export bool RunUnitTests_System()
{
	/** Unit Test Functions */

	// File
	JPT_ENSURE(RunUnitTests_FileIO());

	// FileIO
	JPT_ENSURE(RunUnitTests_FileIO());

	// FilePath
	JPT_ENSURE(RunUnitTests_FilePath());
	JPT_ENSURE(RunUnitTests_FilePathUtils());

	// Time
	JPT_ENSURE(RunUnitTests_Clock());
	JPT_ENSURE(RunUnitTests_StopWatch());

	return true;
}
