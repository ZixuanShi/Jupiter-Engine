// Copyright Jupiter Technologies, Inc. All Rights Reserved.

#include "Logger.h"

#include <Windows.h>

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
		default: return nullptr;
		}
	}

	void Logger::Log(ELogType, int32, const char*, const char*, ...)
	{
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