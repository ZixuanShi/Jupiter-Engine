#pragma once

// Jupiter's Pre-compiled header

// Standard Library
#include <memory>
#include <mutex>
#include <string>
#include <sstream>
#include <time.h>

#if IS_PLATFORM_WIN64
	#include <Windows.h>
#endif

// Jupiter
#include "Core/TypeDefs/TypeDefs.h"
#include "Core/Building/Macros.h"
#include "Core/Debugging/Macros.h"
#include "Core/MemoryManagement/Macros.h"
#include "Core/Logging/Logger.h"