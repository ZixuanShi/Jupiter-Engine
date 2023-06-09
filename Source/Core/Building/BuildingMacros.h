// Copyright Jupiter Technologies, Inc. All Rights Reserved.

#pragma once

/** DLL exporter / importer */
#if JPT_IS_DYNAMICALLY_LINK
	#ifdef JPT_BUILD_DLL
		#define JPT_API  __declspec(dllexport)
	#else	
		#define JPT_API __declspec(dllimport)
	#endif
#else
	#define JPT_API
#endif

/** Used for hacky fix unused parameters */
#define JPT_UNUSED(param) static_cast<void>(param)

/** Turn off optimization */
#define JPT_DEOPTIMIZE __pragma(optimize("", off))

/** Turn on optimization */
#define JPT_REOPTIMIZE __pragma(optimize("", on))

/** Deleter helpers */
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
