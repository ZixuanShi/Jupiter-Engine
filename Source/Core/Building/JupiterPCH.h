#pragma once

// Jupiter's Pre-compiled header

// Standard Library
#include <atomic>
#include <mutex>
#include <memory>
#include <thread>
#include <sstream>
#include <time.h>
#include <utility>
#include <unordered_map>
#include <vector>

#if IS_PLATFORM_WIN64
	#include <Windows.h>
#endif

// Jupiter
#include "Core/Building/BuildingMacros.h"
#include "Core/Types/Types.h"
#include "Core/Logging/Logger.h"
#include "Core/Debugging/DebuggingMacros.h"
#include "Core/MemoryManagement/MemoryOperators.h"
#include "Core/Algorithms/AlgorithmsHeaders.h"
#include "Core/DataStructures/DataStructuresHeaders.h"
#include "Core/Timing/Timer.h"
#include "Core/Timing/TimeProfiler.h"
