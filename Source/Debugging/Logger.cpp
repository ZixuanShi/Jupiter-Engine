// Copyright Jupiter Technologies, Inc. All Rights Reserved.

#if JPT_ENABLE_EDITOR_FEATURES

#include "Logger.h"

#include "Core/Minimal/Macros.h"
#include "Assert.h"

#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>

#if IS_PLATFORM_WIN64
	#include <Windows.h>
#endif

import jpt.String;

namespace jpt
{
	static constexpr size_t kMaxMessageSize = 256;

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
		char messageBuffer[kMaxMessageSize];
		JPT_FORMAT_STRING(messageBuffer, format, ...);
		ProcessMessage(type, line, file, messageBuffer);
	}

	void Logger::Log(ELogType type, int32 line, const char* file, const wchar_t* format, ...)
	{
		wchar_t messageBuffer[kMaxMessageSize];
		JPT_FORMAT_WSTRING(messageBuffer, format, ...);
		ProcessMessage(type, line, file, messageBuffer);
	}

	String Logger::GetStamp(ELogType type, int32 line, const char* file)
	{
		String stamp;
		stamp.Reserve(kMaxMessageSize);

		// Convert to relative path from VS proj, so double-clicking a Log message will redirect to the source code where JPT_LOG got called
		const String fileStr(file);
		stamp += "..\\" + fileStr.SubStr(fileStr.Find("Source"));

		// line number and log type
		stamp += "(" + ToString(line) + "):  \t" + "[" + locGetLogStr(type) + "] ";

		return stamp;
	}

	void Logger::ProcessMessage(ELogType type, int32 line, const char* file, const char* pMessage)
	{
		String contentToLog = GetStamp(type, line, file);
		contentToLog += pMessage;
		contentToLog += "\n";

		SendToOutputWindow(contentToLog.ConstBuffer());
	}

	void Logger::ProcessMessage(ELogType type, int32 line, const char* file, const wchar_t* pMessage)
	{
		const String stamp = GetStamp(type, line, file);

		const size_t wStampStrSize = stamp.Size() + 1;
		wchar_t* wStampStr = new wchar_t[wStampStrSize];
		mbstowcs_s(nullptr, wStampStr, wStampStrSize, stamp.ConstBuffer(), _TRUNCATE);

		WString wStamp;
		wStamp.MoveString(wStampStr);

		wStamp += pMessage;
		wStamp += L"\n";

		SendToOutputWindow(wStamp.ConstBuffer());
	}

	void Logger::SendToOutputWindow(const char* string)
	{
#if IS_PLATFORM_WIN64
		::OutputDebugStringA(string);
#endif
	}

	void Logger::SendToOutputWindow(const wchar_t* wideString)
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

#endif // JPT_ENABLE_EDITOR_FEATURES