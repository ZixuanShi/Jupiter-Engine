#pragma once

#if !JPT_ENABLE_MEMORY_TRACKING
// Not tracking memory version. Fast and efficient
#define JPT_NEW(param) new param

// Deletes a pointer no matter what it's pointing, then sets it to nullptr
#define JPT_DELETE(pPointer)\
			delete pPointer;\
			pPointer = nullptr;

#elif JPT_ENABLE_MEMORY_TRACKING

namespace jpt
{
	JPT_API void InitMemoryLeakDetector();
	JPT_API void ShutdownMemoryLeakDetector();

	JPT_API int32* __cdecl Test();

	// Not tracking memory version. Fast and efficient
	#define JPT_NEW(param) new param
	
	// Deletes a pointer no matter what it's pointing, then sets it to nullptr
	#define JPT_DELETE(pPointer)\
			delete pPointer;\
			pPointer = nullptr;
}

#endif // JPT_ENABLE_MEMORY_TRACKING

// Deletes a pointer if it's valid
#define JPT_SAFE_DELETE(pPointer)\
		if (pPointer)\
		{\
			JPT_DELETE(pPointer)\
		}
