// Copyright Jupiter Technologies, Inc. All Rights Reserved.

#include "JupiterPCH.h"

namespace jpt
{
#if JPT_ENABLE_ASSERTS
	void OnAssertionFailed(int32 line, const char* file, const char* expression, const char* format, ...)
	{
		jpt::string message = "Assertion Failed: ";
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