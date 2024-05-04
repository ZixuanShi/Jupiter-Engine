// Copyright Jupiter Technologies, Inc. All Rights Reserved.

#if IS_DEBUG || IS_DEVELOPMENT

#include "Assert.h"

#include "Core/Minimal/Macros.h"
#include "Logger.h"

#include <stdio.h>
#include <stdarg.h>

import jpt.String;

namespace jpt
{
	void OnAssertionFailed(int line, const char* file, const char* expression, const char* format, ...)
	{
		jpt::String message = "Assertion Failed: ";
		message.Append(expression);

		if (format)
		{
			message.Append(", ");

			char messageBuffer[512];
			JPT_FORMAT_STRING(messageBuffer, format, ...);

			message.Append(messageBuffer);
		}

		jpt::Logger::GetInstance().Log(jpt::Logger::ELogType::Error, line, file, message.ConstBuffer());
	}

	void OnAssertionFailed(int line, const char* file, const char* expression)
	{
		OnAssertionFailed(line, file, expression, nullptr);
	}

	void DebugBreak()
	{
#if IS_PLATFORM_WIN64
		__debugbreak();
#else
#error "DebugBreak not implemented for this platform"
#endif
	}
}
#endif // IS_DEBUG || IS_DEVELOPMENT