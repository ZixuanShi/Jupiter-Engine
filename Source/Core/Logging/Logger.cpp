// Copyright Jupiter Technologies, Inc. All Rights Reserved.

#include "JupiterPCH.h"
#include "Logger.h"

#include <iostream>

namespace jpt
{
	const char* locGetLogStr(const Logger::ELogType& type)
	{
		switch (type)
		{
		case Logger::ELogType::Log:			return "Log";
		case Logger::ELogType::SystemInfo:	return "SystemInfo";
		case Logger::ELogType::Warning:		return "Warning";
		case Logger::ELogType::Error:		return "Error";
		default: return nullptr;
		}
	}

	void Logger::Log(const ELogType& type, int32 line, const char* file, const char* message, ...)
	{
		char messageBuffer[512];

		va_list args;
		va_start(args, message);

		vsprintf_s(messageBuffer, message, args);

		va_end(args);

		// Get valid file path
		jpt::string contentToLog(file);
		contentToLog = contentToLog.substr(contentToLog.find_last_of("\\") + 1);
		contentToLog += ", line (" + jpt::to_string(line) + "):  \t" + "[" + locGetLogStr(type) + "] " + messageBuffer + "\n";

		// Log to the output window
		::OutputDebugStringA(contentToLog.c_str());
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