#include "JupiterPCH.h"
#include "MemoryTracking.h"

#if JPT_ENABLE_MEMORY_TRACKING

void jpt::InitMemoryLeakDetector()
{
}

void jpt::ShutdownMemoryLeakDetector()
{
}

int32* jpt::Test()
{
	JPT_LOG("Hi");
	return new int32(5);
}

#endif // JPT_ENABLE_MEMORY_TRACKING
