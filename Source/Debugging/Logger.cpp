// Copyright Jupiter Technologies, Inc. All Rights Reserved.

#if JPT_ENABLE_EDITOR_FEATURES

#include "Logger.h"

#include "Core/Minimals/Macros.h"
#include "Assert.h"

#include <stdio.h>

#if IS_PLATFORM_WIN64
	#include <Windows.h>
#endif

import jpt.String;
import jpt.TimingUtilsDeprecated;

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
		char messageBuffer[512];
		FORMAT_STRING(messageBuffer, format, ...);
		ProcessMessage(type, line, file, messageBuffer);
	}

	void Logger::PrintToConsole(const char* string)
	{
#if IS_PLATFORM_WIN64
		::OutputDebugStringA(string);
#endif
	}

	void Logger::PrintToConsole(const wchar_t* wideString)
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

	void Logger::ProcessMessage(ELogType type, int32 line, const char* file, const char* pMessage)
	{
		const String fileStr(file);

		String contentToLog;
		contentToLog.Reserve(256);

		// Time
		//contentToLog += jpt::ToString(SystemClock::now()) + ". ";

		// C++ file name. Get rid of previous folders
		//contentToLog += fileStr.SubStr(fileStr.FindLastOf("\\") + 1);
		contentToLog += fileStr;	// If use full path, double clicking the output string in output window will direct to the LOG() call code

		// line number and log type
		contentToLog += "(" + jpt::ToString(line) + "):  \t" + "[" + locGetLogStr(type) + "] ";

		// Content message
		contentToLog += pMessage;
		contentToLog += "\n";

		// Log to the output window
		PrintToConsole(contentToLog.ConstBuffer());
	}
}

#endif // JPT_ENABLE_EDITOR_FEATURES