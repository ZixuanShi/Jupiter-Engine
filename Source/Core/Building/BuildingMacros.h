#pragma once

// DLL exporter / importer
#ifdef IS_PLATFORM_WIN64
	#ifdef JPT_BUILD_DLL
		#define JPT_API __declspec(dllexport)
	#else	
		#define JPT_API __declspec(dllimport)
#endif
#else
	#error Jupiter only supports Windows
#endif

// Used for hacky fix unused parameters
#define JPT_UNUSED(param) static_cast<void>(param)

// Turn off optimization
#define JPT_DEOPTIMIZE __pragma(optimize("", off))

// Turn on optimization
#define JPT_REOPTIMIZE __pragma(optimize("", on))

// Delete from memories
#define JPT_DELETE(pPointer)\
			delete pPointer;\
			pPointer = nullptr;

#define JPT_DELETE_ARRAY(pPointer)\
			delete[] pPointer;\
			pPointer = nullptr;

#define JPT_SAFE_DELETE(pPointer)\
		if (pPointer)\
		{\
			JPT_DELETE(pPointer);\
		}

#define JPT_SAFE_DELETE_ARRAY(pPointer)\
		if (pPointer)\
		{\
			JPT_DELETE_ARRAY(pPointer);\
		}