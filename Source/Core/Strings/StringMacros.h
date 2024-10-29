// Copyright Jupiter Technologies, Inc. All Rights Reserved.

#pragma once

import jpt.Utilities;

/** Combines two strings 
	@example #define TEST(x) JPT_COMBINE_STR("Hello ", #x)	// x can be anything
	@example #define TEST(x) JPT_COMBINE_STR(L"你好 ", #x) */
#define JPT_COMBINE_STR(A, B)   A##B

/** Converts a const char* to const wchar_t* by prefixing 'L' */
#define JPT_TO_WSTRING(cStr) L##cStr

/** This solves the issue when dealing with templated string class functions with raw string literals involved. No Heap Memory Allocation at all
	@return   C-Style string for different Char Type but Has the exact input string literals.
	@example: const char* cstr = JPT_GET_PROPER_STRING(char, Hello World);		  // cstr will be "Hello World"
	@example: const wchar_t* wcstr = JPT_GET_PROPER_STRING(wchar_t, Hello World); // wcstr will be L"Hello World" */
#define JPT_GET_PROPER_STRING(TChar, SourceStr)              \
	[]() -> const TChar*                                     \
	{                                                        \
		const TChar* pString = nullptr;                      \
		if constexpr (jpt::AreSameType<TChar, char>)         \
		{                                                    \
			pString = #SourceStr;                            \
		}                                                    \
		else if constexpr (jpt::AreSameType<TChar, wchar_t>) \
		{                                                    \
			pString = JPT_TO_WSTRING(#SourceStr);            \
		}                                                    \
		else                                                 \
		{                                                    \
			JPT_ASSERT(false, "Unsupported TChar");          \
		}                                                    \
		return pString;                                      \
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
		va_list args;						  \
		va_start(args, format);               \
		vsprintf_s(buffer, format, args);     \
		va_end(args);                         

#define JPT_FORMAT_WSTRING(buffer, format, ...)\
		va_list args;                          \
		va_start(args, format);                \
		vswprintf_s(buffer, format, args);     \
		va_end(args);    