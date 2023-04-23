#include "JupiterPCH.h"
#include "MemoryTracking.h"

#if JPT_ENABLE_MEMORY_TRACKING

void jpt::InitMemoryLeakDetector()
{
}

void jpt::ShutdownMemoryLeakDetector()
{
}

void* operator new(size_t size)
{
	JPT_LOG("Yes");
	return malloc(size);
}

#endif // JPT_ENABLE_MEMORY_TRACKING

