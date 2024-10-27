// Copyright Jupiter Technologies, Inc. All Rights Reserved.

export module jpt.Status;

export namespace jpt
{
	enum class Status
	{
		Pending,		// Not started
		Running,		// In progress
		Complete,		// Finished

		Failure,
		Success,
	};
}