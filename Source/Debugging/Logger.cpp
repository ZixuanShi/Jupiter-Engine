// Copyright Jupiter Technologies, Inc. All Rights Reserved.

#include "Debugging/Logger.h"

#if IS_LOGGER_ENABLED

#include "Core/Strings/StringMacros.h"
#include "Debugging/Assert.h"
#include "Applications/App/Application_Base.h"

#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>
#include <mutex>

#if IS_PLATFORM_WIN64
	#include <Windows.h>
#endif

import jpt.Clock;
import jpt.DateTime;
import jpt.File.IO;
import jpt.File.Path;
import jpt.System.Paths;
import jpt.Environment;
import jpt.String.Helpers;

namespace jpt
{
	static constexpr size_t kMaxMessageSize = 1024;
	static const File::Path kLogFilePath = { File::Source::Saved, "Log.txt" };

	static const char* locGetLogStr(Logger::ELogType type)
	{
		switch (type)
		{
			case Logger::ELogType::Log:			return "Log";
			case Logger::ELogType::Info:		return "Info";
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

	String Logger::GetInfoStamp(ELogType type, int32 line, const char* file)
	{
		String stamp;
		stamp.Reserve(kMaxMessageSize);

		// Convert to relative path from VS proj, so double-clicking a Log message will redirect to the source code where JPT_LOG got called
		const String fileStr(file);
		stamp += ".." + File::GetSeparator<String>() + fileStr.SubStr(fileStr.Find("Source"));

		// line number and log type
		stamp += "(" + ToString(line) + "):  \t" + "[" + locGetLogStr(type) + "] ";

		return stamp;
	}

	String Logger::GetTimeStamp()
	{
		const DateTime now = Clock::GetCurrentDateTime();
		const String nowStr = now.ToString() + " - ";
		return nowStr;
	}

	void Logger::ProcessMessage(ELogType type, int32 line, const char* file, const char* pMessage)
	{
		String contentToLog = GetInfoStamp(type, line, file);
		contentToLog += pMessage;
		contentToLog += "\n";

		if (jpt::IsDebuggerPresent())
		{
			SendToOutputWindow(contentToLog.ConstBuffer());
		}

		if (GetApplication()->IsPreInitialized())
		{
			File::AppendTextFile(kLogFilePath, GetTimeStamp() + contentToLog);
		}
	}

	void Logger::ProcessMessage(ELogType type, int32 line, const char* file, const wchar_t* pMessage)
	{
		const String contentToLog = GetInfoStamp(type, line, file);
		WString ContentToLogW = ToWString(contentToLog);

		ContentToLogW += pMessage;
		ContentToLogW += L"\n";

		if (jpt::IsDebuggerPresent())
		{
			SendToOutputWindow(ContentToLogW.ConstBuffer());
		}

		if (GetApplication()->IsPreInitialized())
		{
			File::AppendTextFile(kLogFilePath, GetTimeStamp() + ToString(ContentToLogW));
		}
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

#endif