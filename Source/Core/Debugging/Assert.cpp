// Copyright Jupiter Technologies, Inc. All Rights Reserved.

#include "Assert.h"

#include "Logger.h"

#include <stdarg.h>

// TODO: Replace with jpt string when finished
#include <string>

namespace jpt
{
#if JPT_ENABLE_ASSERTS
	void OnAssertionFailed(int32 line, const char* file, const char* expression, const char* format, ...)
	{
		std::string message = "Assertion Failed: ";
		message.append(expression);

		if (format)
		{
			message.append(", ");

			char messageBuffer[512];

			va_list args;
			va_start(args, format);

			vsprintf_s(messageBuffer, format, args);

			va_end(args);

			message.append(messageBuffer);
		}

		jpt::Logger::GetInstance().Log(jpt::Logger::ELogType::Error, line, file, message.c_str());
	}

	void OnAssertionFailed(int32 line, const char* file, const char* expression)
	{
		OnAssertionFailed(line, file, expression, nullptr);
	}
#endif
}