#pragma once

#if JPT_ENABLE_MEMORY_TRACKING
#include "MemoryTracking.h"
	//// Replacements for new and new[]
	//#define JPT_NEW(type) ::operator new(__FILE__, __LINE__) type
	//#define JPT_NEW_ARRAY(type, size) ::operator new(__FILE__, __LINE__) type[size]
	//
	//// Replacements for delete and delete[]
	//#define JPT_DELETE(pPointer)\
	//		::operator delete(__FILE__, __LINE__) pPointer;\
	//		pPointer = nullptr;
	//
	//#define JPT_DELETE_ARRAY(pArray)\
	//		::operator delete[](__FILE__, __LINE__) pArray;\
	//		pArray = nullptr;
	//
	// Needs to be called at the top of main()
	#define JPT_INIT_MEMORY_TRACKER()		jpt::InitMemoryLeakDetector();
	
	// Needs to be called at the bottom of main()
	#define JPT_SHUTDOWN_MEMORY_TRACKER()	jpt::ShutdownMemoryLeakDetector();

//#else
	// Not tracking memory version
	#define JPT_NEW(param) new param
	#define JPT_NEW_ARRAY(type, size) new type[size]

	// Deletes a pointer no matter what it's pointing, then sets it to nullptr
	#define JPT_DELETE(pPointer)\
			delete pPointer;\
			pPointer = nullptr;
	
	#define JPT_DELETE_ARRAY(pArray)\
			delete[] pArray;\
			pArray = nullptr;
#endif

// Deletes a pointer if it's valid
#define JPT_SAFE_DELETE(pPointer)\
		if (pPointer)\
		{\
			JPT_DELETE(pPointer)\
		}

#define JPT_SAFE_DELETE_ARRAY(pArray)\
		if (pArray)\
		{\
			JPT_DELETE_ARRAY(pArray)\
		}