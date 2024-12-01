// Copyright Jupiter Technologies, Inc. All Rights Reserved.

#include "Assert.h"

#if ASSERT_ENABLED

#include "Core/Strings/StringMacros.h"

#include <assert.h>
#include <stdio.h>
#include <stdarg.h>

import jpt.String;

namespace jpt
{
	void OnAssertionFailed(int line, const char* file, const char* expression, const char* format, ...)
	{
		if (!g_AssertCallback)
		{
			assert(false);
			return;
		}

		jpt::String message = "Assertion Failed: ";
		message.Append(expression);

		if (format)
		{
			message.Append(", ");

			char messageBuffer[512];
			JPT_FORMAT_STRING(messageBuffer, format, ...);

			message.Append(messageBuffer);
		}

		g_AssertCallback(line, file, message.ConstBuffer());
	}

	void OnAssertionFailed(int line, const char* file, const char* expression)
	{
		OnAssertionFailed(line, file, expression, nullptr);
	}
}
#endif // ASSERT_ENABLED