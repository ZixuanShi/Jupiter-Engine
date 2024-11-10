// Copyright Jupiter Technologies, Inc. All Rights Reserved.

export module jpt.QueueFamilyIndices;

import jpt.TypeDefs;
import jpt.Optional;

export namespace jpt
{
	struct QueueFamilyIndices
	{
		Optional<uint32> graphicsFamily;
		Optional<uint32> presentFamily;

		bool IsComplete() const
		{
			return graphicsFamily.HasValue() && presentFamily.HasValue();
		}
	};
}