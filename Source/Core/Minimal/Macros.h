// Copyright Jupiter Technologies, Inc. All Rights Reserved.

#pragma once

#pragma region Building
/** Used for hacky fix unused parameters */
#define JPT_IGNORE(...) __VA_ARGS__

/** Turn off optimization */
#define JPT_DEOPTIMIZE __pragma(optimize("", off))

/** Turn on optimization */
#define JPT_REOPTIMIZE __pragma(optimize("", on))

#pragma endregion

#pragma region Memory

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

#pragma endregion

#pragma region Logging

/**< Return, with Error logging */
#define JPT_VERIFY(condition, returnValue, ...)\
	if (!(condition))\
	{\
		if (JPT_HAS_ARGS(__VA_ARGS__))\
		{\
			JPT_ERROR("%s", __VA_ARGS__);\
		}\
		else\
		{\
			JPT_ERROR("Failed: %s", #condition);\
		}\
		return returnValue;\
	}

#define JPT_ENSURE(condition, ...)\
	JPT_VERIFY(condition, false, __VA_ARGS__)

/**< Slient Return */
#define JPT_RETURN_IF(condition, returnValue)\
	if ((condition))\
	{\
		return returnValue;\
	}

#define JPT_EXIT_IF(condition)\
	JPT_RETURN_IF(condition, void())

#pragma endregion

#pragma region String

/** Combines two strings 
	@example #define TEST(x) JPT_COMBINE_STR("Hello ", #x)	// x can be anything
	@example #define TEST(x) JPT_COMBINE_STR(L"你好 ", #x) */
#define JPT_COMBINE_STR(A, B)   A##B

/** Converts a const char* to const wchar_t* by prefixing 'L' */
#define JPT_TO_WSTRING(cStr) L##cStr

/** This solves the issue when dealing with templated string class functions with raw string literals involved. No Heap Memory Allocation at all
	@return   C-Style string for different Char Type but contains the exact input string literals.
	@example: const char* cstr = JPT_GET_PROPER_STRING(char, Hello World);		  // cstr will be "Hello World"
	@example: const wchar_t* wcstr = JPT_GET_PROPER_STRING(wchar_t, Hello World); // wcstr will be L"Hello World" */
#define JPT_GET_PROPER_STRING(TChar, SourceStr)\
	[]() -> const TChar* \
	{\
		const TChar* pString = nullptr; \
		if constexpr (jpt::IsSameType<TChar, char>)\
		{\
			pString = #SourceStr;\
		}\
		else if constexpr (jpt::IsSameType<TChar, wchar_t>)\
		{\
			pString = JPT_TO_WSTRING(#SourceStr);\
		}\
		else\
		{\
			JPT_ASSERT(false, "Unsupported TChar");\
		}\
		return pString;\
	}()

/** Formats a buffer as string 
	@example:
			#include <stdarg.h>
			#include <stdio.h>

			void Foo(const char* format, ...)	// lambda works too
			{
				char buffer[64];	// any size you want
				JPT_FORMAT_STRING(buffer, format, ...);

				// Do something with buffer
			} */
#define JPT_FORMAT_STRING(buffer, format, ...)\
	va_list args;\
	va_start(args, format);\
	vsprintf_s(buffer, format, args);\
	va_end(args)\

#define JPT_FORMAT_WSTRING(buffer, format, ...)\
	va_list args;\
	va_start(args, format);\
	vswprintf_s(buffer, format, args);\
	va_end(args)\

#pragma endregion

#pragma region VA_ARGS

/** @return true if a macro's variadic arguments has passed in parameters. false if it's empty
	@example:
	#define MACRO_WITH_VARIADIC_ARGUMENTS(...)			{ if (JPT_HAS_ARGS(__VA_ARGS__)) { DoStuff(); } }	*/
#define JPT_HAS_ARGS(...) jpt::HasAnyArgs(__VA_ARGS__)

/** @return	Count of arguments in __VA_ARGS__ */
#define JPT_ARGS_COUNT(...)	jpt::GetArgsCount(__VA_ARGS__)

#pragma endregion

/** @return Count of a plain array.
	@example:
	int intArray[12];
	int intArrayCount = JPT_ARRAY_COUNT(intArray); */
template <typename T, unsigned int kNumber>
char(&ArrayCountHelper(const T(&)[kNumber]))[kNumber + 1];
#define JPT_ARRAY_COUNT(inArray) (sizeof(ArrayCountHelper(inArray)) - 1)