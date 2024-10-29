// Copyright Jupiter Technologies, Inc. All Rights Reserved.

module;

#include "Core/Types/Enum.h"

export module jpt.Status;

export namespace jpt
{
	JPT_ENUM_UINT8(Status,
		Pending  = 1 << 0,		// Not started
		Running  = 1 << 1,		// In progress

		Failure  = 1 << 2,
		Success  = 1 << 3,

		//Complete = Failure | Success
	);
}