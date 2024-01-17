// Copyright Jupiter Technologies, Inc. All Rights Reserved.

#pragma once

/** Used for hacky fix unused parameters */
#define JPT_IGNORE(param) static_cast<void>(param)

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

/** If the condition has occured, log an messageand return a value
	@param condition:	The expression of the operation
	@param returnValue: The value to return if condition occured
	@param message:		The message to log if condition occured */
#define JPT_RETURN_VALUE_IF_ERROR(condition, returnValue, ...)\
	if ((condition))\
	{\
		JPT_ERROR(__VA_ARGS__);\
		return returnValue;\
	}

#define JPT_RETURN_VALUE_IF(condition, returnValue)\
	if ((condition))\
	{\
		return returnValue;\
	}

#define JPT_RETURN_FALSE_IF_ERROR(condition, ...)\
	JPT_RETURN_VALUE_IF_ERROR(condition, false, __VA_ARGS__);

#define JPT_RETURN_FALSE_IF(condition)\
	JPT_RETURN_VALUE_IF(condition, false);

#define JPT_RETURN_IF_ERROR(condition, ...)\
	JPT_RETURN_VALUE_IF_ERROR(condition, void(), __VA_ARGS__);

#define JPT_RETURN_IF(condition)\
	JPT_RETURN_VALUE_IF(condition, void());

#define JPT_ERROR_IF(condition, ...)\
	if ((condition))\
	{\
		JPT_ERROR(__VA_ARGS__);\
	}

#define JPT_LOG_IF(condition, ...)\
	if ((condition))\
	{\
		JPT_LOG(__VA_ARGS__);\
	}

/** Helper for clean and delete.
	@param pPointer: Assumed to have Clean() function implemented */
#define JPT_CLEAN_THEN_DELETE(pPointer)\
	pPointer->Clean();\
	JPT_DELETE(pPointer);

/** Helper for safe cleanand delete.
	@param pPointer: Assumed to have Clean() function implemented */
#define JPT_SAFE_CLEAN_THEN_DELETE(pPointer)\
	if(pPointer)\
	{\
		pPointer->Clean();\
	}\
	JPT_SAFE_DELETE(pPointer);

/** Combines two strings */
#define JPT_COMBINE_STR(A, B)   A##B

/** Converts a const char* to const wchar_t* by prefixing 'L' */
#define JPT_TO_WSTRING(cStr) L##cStr

/** This solves the issue when dealing with templated string class functions with raw string literals involved. No Heap Memory Allocation at all
	@return   C-Style string for different Char Type but contains the exact input string literals.
	@example: const char* cstr = JPT_GET_PROPER_STRING(char, Hello World);		  // cstr will be "Hello World"
	@example: const wchar_t* wcstr = JPT_GET_PROPER_STRING(wchar_t, Hello World); // wcstr will be L"Hello World" */
#define JPT_GET_PROPER_STRING(CharT, SourceStr)\
	[]() -> const CharT* \
	{\
		const CharT* pString = nullptr; \
		if constexpr (jpt::IsSameType<CharT, char>)\
		{\
			pString = #SourceStr;\
		}\
		else if (jpt::IsSameType<CharT, wchar_t>)\
		{\
			pString = JPT_TO_WSTRING(#SourceStr);\
		}\
		else\
		{\
			JPT_ASSERT(false, "Unsupported CharT");\
		}\
		return pString;\
	}()

/** @return true if a macro's variadic arguments has passed in parameters. false if it's empty
	@example:
	#define MACRO_WITH_VARIADIC_ARGUMENTS(...)			{ if (JPT_HAS_ARGS(__VA_ARGS__)) { DoStuff(); } }	*/
#define JPT_HAS_ARGS(...) (""#__VA_ARGS__[0] != '\0')

/** @return Count of a plain array.
	@example:
	int intArray[12];
	int intArrayCount = JPT_ARRAY_COUNT(intArray); */
template <typename T, unsigned int kNumber>
char(&ArrayCountHelper(const T(&)[kNumber]))[kNumber + 1];
#define JPT_ARRAY_COUNT(inArray) (sizeof(ArrayCountHelper(inArray)) - 1)