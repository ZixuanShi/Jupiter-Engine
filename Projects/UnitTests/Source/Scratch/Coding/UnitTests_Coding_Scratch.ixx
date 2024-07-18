// Copyright Jupiter Technologies, Inc. All Rights Reserved.

module;

#include "Core/Minimal/CoreHeaders.h"

#include <thread>
#include <mutex>
#include <atomic>
#include <condition_variable>

export module UnitTests_Coding_Scratch;

import jpt.CoreModules;
import jpt.CPUInfo;

std::mutex g_mutex;

void WorkerThread(int& i)
{
	std::lock_guard<std::mutex> lock(g_mutex);

	JPT_LOG(i);
	++i;
}

export bool UnitTests_Coding_Scratch()
{
	//jpt::DynamicArray<std::thread> workers;
	//int count = 0;

	//for (uint32 i = 0; i < jpt::GetNumCores(); ++i)
	//{
	//	workers.EmplaceBack(std::thread(WorkerThread, std::ref(count)));
	//}

	//for (uint32 i = 0; i < 8; ++i)
	//{
	//	workers[i].join();
	//}

	//JPT_LOG(count);

	return true;
}