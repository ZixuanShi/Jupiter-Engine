// Copyright Jupiter Technologies, Inc. All Rights Reserved.

module;

#include "Core/Types/Enum.h"

export module jpt.Status;

export namespace jpt
{
	JPT_ENUM_UINT8(Status,
		Pending  = 1 << 0,		// Not started
		Running  = 1 << 1,		// In progress

		Failure = 1 << 2,		// Completed with errors
		Success = 1 << 3,		// Completed successfully
		Warning = 1 << 4,		// Completed with warnings

		Dummy = 255,
		//Dummy2 = Dummy,

		//Foo = Success,			// Alias
		//Complete = Failure | Success | Warning
	);
}