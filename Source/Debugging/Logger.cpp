// Copyright Jupiter Technologies, Inc. All Rights Reserved.

#if !IS_RELEASE

#include "Debugging/Logger.h"

#include "Core/Minimal/CoreMacros.h"
#include "Debugging/Assert.h"

#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>
#include <mutex>

#if IS_PLATFORM_WIN64
	#include <Windows.h>
#endif

import jpt.FileIO;
import jpt.File.Path;
import jpt.System.Paths;
import jpt.String.Helpers;

namespace jpt
{
	static constexpr size_t kMaxMessageSize = 1024;
	static const File::Path kLogFilePath = { File::ESource::Saved, "Log.txt" };

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
		stamp += ".." + File::GetSeparator<String>() + fileStr.SubStr(fileStr.Find("Source"));

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
		AppendToSaveFile(contentToLog);
	}

	void Logger::ProcessMessage(ELogType type, int32 line, const char* file, const wchar_t* pMessage)
	{
		const String contentToLog = GetStamp(type, line, file);
		WString ContentToLogW = ToWString(contentToLog);

		ContentToLogW += pMessage;
		ContentToLogW += L"\n";

		SendToOutputWindow(ContentToLogW.ConstBuffer());
		AppendToSaveFile(ContentToLogW);
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

	void Logger::AppendToSaveFile(const String& string)
	{
		File::AppendTextFile(kLogFilePath, string);
	}

	void Logger::AppendToSaveFile(const WString& wideString)
	{
		File::AppendTextFile(kLogFilePath, ToString(wideString));
	}

	Logger& Logger::GetInstance()
	{
		static Logger s_logger;
		return s_logger;
	}
}

#endif // !IS_RELEASE