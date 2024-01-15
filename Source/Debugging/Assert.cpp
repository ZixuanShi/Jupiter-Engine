// Copyright Jupiter Technologies, Inc. All Rights Reserved.

#if JPT_ENABLE_DEBUG_FEATURES

#include "Assert.h"

#include "Logger.h"

#include <stdio.h>
#include <stdarg.h>

import jpt.String;

namespace jpt
{
	void OnAssertionFailed(int32 line, const char* file, const char* expression, const char* format, ...)
	{
		jpt::String message = "Assertion Failed: ";
		message.Append(expression);

		if (format)
		{
			message.Append(", ");

			char messageBuffer[512];

			va_list args;
			va_start(args, format);

			vsprintf_s(messageBuffer, format, args);

			va_end(args);

			message.Append(messageBuffer);
		}

		jpt::Logger::GetInstance().Log(jpt::Logger::ELogType::Error, line, file, message.ConstBuffer());
	}

	void OnAssertionFailed(int32 line, const char* file, const char* expression)
	{
		OnAssertionFailed(line, file, expression, nullptr);
	}
}
#endif // JPT_ENABLE_DEBUG_FEATURES