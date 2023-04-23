#pragma once

#if JPT_ENABLE_MEMORY_TRACKING

void* operator new(size_t size);

namespace jpt
{
	void JPT_API InitMemoryLeakDetector();
	void JPT_API ShutdownMemoryLeakDetector();
}

#endif // JPT_ENABLE_MEMORY_TRACKING