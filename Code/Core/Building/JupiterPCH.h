#pragma once

// Jupiter's Pre-compiled header

// Standard Library
#include <atomic>
#include <mutex>
#include <memory>
#include <thread>
#include <string>
#include <sstream>
#include <time.h>
#include <utility>
#include <unordered_map>
#include <vector>

#if IS_PLATFORM_WIN64
	#include <Windows.h>
#endif

// Jupiter
#include "Core/TypeDefs/TypeDefs.h"
#include "Core/Building/Macros.h"
#include "Core/Algorithms/Hash.h"
#include "Core/Debugging/Macros.h"
#include "Core/MemoryManagement/MemoryTracking.h"
#include "Core/Logging/Logger.h"
#include "Core/Timing/Timer.h"
#include "Core/Timing/TimeProfiler.h"