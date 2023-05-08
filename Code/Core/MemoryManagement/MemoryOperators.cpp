#include "JupiterPCH.h"
#include "MemoryOperators.h"

#if JPT_ENABLE_MEMORY_TRACKING

namespace jpt
{
	void InternalCallTrackedDelete(void* pPointer)
	{
		//MemoryTracker::GetInstance()->ReduceEntry(pPointer);
		delete pPointer;
		pPointer = nullptr;
	}
	void InternalCallTrackedDeleteArray(void* pPointer)
	{
		delete[] pPointer;
		pPointer = nullptr;
	}
}

#endif // JPT_ENABLE_MEMORY_TRACKING
