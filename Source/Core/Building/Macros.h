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
#define JPT_RETURN_VALUE_IF_LOG(condition, returnValue, ...)\
	if ((condition))\
	{\
		JPT_LOG(__VA_ARGS__);\
		return returnValue;\
	}

#define JPT_RETURN_VALUE_IF(condition, returnValue)\
	if ((condition))\
	{\
		return returnValue;\
	}

#define JPT_RETURN_FALSE_IF_LOG(condition, ...)\
	JPT_RETURN_VALUE_IF_LOG(condition, false, __VA_ARGS__);

#define JPT_RETURN_FALSE_IF(condition)\
	JPT_RETURN_VALUE_IF(condition, false);

#define JPT_RETURN_IF_LOG(condition, ...)\
	JPT_RETURN_VALUE_IF_LOG(condition, void(), __VA_ARGS__);

#define JPT_RETURN_IF(condition)\
	JPT_RETURN_VALUE_IF(condition, void());

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

/** __FILE__ but with L"XXX" wide string */
#define JPT_WIDE2(x) L##x
#define JPT_WIDE1(x) JPT_WIDE2(x)
#define JPT_WFILE JPT_WIDE1(__FILE__)

/** Converts a const char* to const wchar_t* */
#define JPT_COMBINE_STR(A, B)   A##B
#define JPT_TO_WSTRING(cStr) L##cStr

/** @return true if a macro's variadic arguments is not empty. false if the macro doesn't have optional arguments
	@example:
	#define MACRO_WITH_VARIADIC_ARGUMENTS(...)			{ if (JPT_HAS_ARGS(__VA_ARGS__)) { DoStuff(); } }	*/
#define JPT_HAS_ARGS(...) (""#__VA_ARGS__[0] != '\0')

/** @return Count of a plain array.
	@example:
	int intArray[12];
	int intArrayCount = JPT_ARRAY_COUNT(intArray); */
template <typename Type, unsigned int Number>
char(&ArrayCountHelper(const Type(&)[Number]))[Number + 1];
#define JPT_ARRAY_COUNT( inArray ) (sizeof(ArrayCountHelper(inArray)) - 1)

/** Compile time outputs an error containing the argument value
	@example: CT_PRINT(sizeof(int)); will output char [4] */
#define JPT_CT_PRINT( X ) { char JPT_CT_PRINT_TRICK[(X)]; CT_PRINT_TRICK = 1; }