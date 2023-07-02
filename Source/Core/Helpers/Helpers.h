// Copyright Jupiter Technologies, Inc. All Rights Reserved.

#pragma once

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

/** Helper for cleanand delete.
    @param pPointer: Assumed to have Clean() function implemented */
#define JPT_CLEAN_THEN_DELETE(pPointer)\
	pPointer->Clean();\
	JPT_DELETE(pPointer);

/** __FILE__ but with L"XXX" wide string */
#define JPT_WIDE2(x) L##x
#define JPT_WIDE1(x) JPT_WIDE2(x)
#define JPT_WFILE JPT_WIDE1(__FILE__)

/** Converts a const char* to const wchar_t* */
#define JPT_COMBINE_STR(A, B)   A##B
#define JPT_TO_WSTRING(cStr) L##cStr

/** @return true if a macro's variadic arguments is not empty. false if the macro doesn't have optional arguments */
#define JPT_HAS_ARGS(...) (""#__VA_ARGS__[0] != '\0')