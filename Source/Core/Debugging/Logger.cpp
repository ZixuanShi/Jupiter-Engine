// Copyright Jupiter Technologies, Inc. All Rights Reserved.

#include "Logger.h"

#include "Assert.h"

#include <stdio.h>

#if IS_PLATFORM_WIN64
	#include <Windows.h>
#endif

import jpt.String;
import jpt.TimingUtils;

namespace jpt
{
	const char* locGetLogStr(Logger::ELogType type)
	{
		switch (type)
		{
			case Logger::ELogType::Log:			return "Log";
			case Logger::ELogType::SystemInfo:	return "SystemInfo";
			case Logger::ELogType::Warning:		return "Warning";
			case Logger::ELogType::Error:		return "Error";
			default:  JPT_ASSERT(false, "Unrecognized Log category"); return nullptr;
		}
	}

	void Logger::Log(ELogType type, int32 line, const char* file, const char* format, ...)
	{
		// Parse info from parameters by format
		char messageBuffer[512];
		va_list args;
		va_start(args, format);
		vsprintf_s(messageBuffer, format, args);
		va_end(args);

		const String messageStr(messageBuffer);
		const String fileStr(file);

		String contentToLog;
		contentToLog.Reserve(256);

		// Time
		contentToLog += jpt::ToString(SystemClock::now()) + ". ";

		// C++ file name. Get rid of previous folders
		contentToLog += fileStr.SubStr(fileStr.FindLastOf("\\") + 1);

		// line number and log type
		contentToLog += ", line (" + jpt::ToString(line) + "):  \t" + "[" + locGetLogStr(type) + "] ";

		// Content message
		contentToLog += messageStr + "\n";

		// Log to the output window
		Log(contentToLog.ConstBuffer());
	}

	void Logger::Log(const char* string)
	{
#if IS_PLATFORM_WIN64
		::OutputDebugStringA(string);
#endif
	}

	void Logger::Log(const wchar_t* wideString)
	{
#if IS_PLATFORM_WIN64
		::OutputDebugStringW(wideString);
#endif
	}

	Logger& Logger::GetInstance()
	{
		static Logger s_logger;
		return s_logger;
	}
}