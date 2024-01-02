// Copyright Jupiter Technologies, Inc. All Rights Reserved.

#include "Logger.h"

namespace jpt
{
	void Logger::Log(ELogType, /*CurrentTime,*/ int32, const char*, const char*, ...)
	{
	}

	void Logger::Log(const char*)
	{
	}

	void Logger::Log(const wchar_t*)
	{
	}

	Logger& Logger::GetInstance()
	{
		static Logger s_logger;
		return s_logger;
	}
}