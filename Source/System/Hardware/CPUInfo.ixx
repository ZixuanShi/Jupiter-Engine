// Copyright Jupiter Technologies, Inc. All Rights Reserved.

module;

#include <thread>

export module jpt.CPUInfo;

import jpt.TypeDefs;

namespace jpt
{
	export uint32 GetNumCores()
	{
		return std::thread::hardware_concurrency();
	}
}