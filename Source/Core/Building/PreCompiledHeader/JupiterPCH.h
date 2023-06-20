// Copyright Jupiter Technologies, Inc. All Rights Reserved.

#pragma once

// Jupiter's Pre-compiled header

#if IS_PLATFORM_WIN64
#include <Windows.h>
#include <d3d12.h>
#include <dxgi1_6.h>
#include <D3Dcompiler.h>
#include <DirectXMath.h>
#include <wrl.h>
#include <shellapi.h>
	#if IS_DEBUG
		#include <dxgidebug.h>
	#endif
#endif

// Standard Library
#include <atomic>
#include <mutex>
#include <memory>
#include <thread>
#include <sstream>
#include <time.h>
#include <utility>

// Jupiter
#include "Core/Building/BuildingMacros.h"
#include "Core/Debugging/DebuggingMacros.h"
#include "Core/Types/Types.h"
#include "Core/Algorithms/AlgorithmsHeaders.h"
#include "Core/DataStructures/DataStructuresHeaders.h"
#include "Core/Types/TypesHeader.h"
#include "Core/Math/MathHeaders.h"
#include "Core/Timing/TimingHeader.h"
#include "Core/Logging/Logger.h"
#include "Core/Helpers/Helpers.h"
#include "Core/System/SystemHeaders.h"
#include "Core/DataDriven/DataDrivenHeaders.h"

#include "Rendering/PipelineObjects/PipelineObjectsHeaders.h"
