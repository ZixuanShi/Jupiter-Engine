// Copyright Jupiter Technologies, Inc. All Rights Reserved.

module;

#include <thread>

export module jpt.CPUInfo;

import jpt.TypeDefs;
import jpt.String;

export namespace jpt
{
	class CPUInfo
	{
	private:
		struct CPUData
		{

		};

	public:
	};

	uint32 GetNumCores()
	{
		return std::thread::hardware_concurrency();
	}
}