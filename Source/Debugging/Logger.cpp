// Copyright Jupiter Technologies, Inc. All Rights Reserved.

#include "Debugging/Logger.h"

#if IS_LOGGER_ENABLED

#include "Core/Strings/StringMacros.h"
#include "Core/Validation/Assert.h"

#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>

#if IS_PLATFORM_WINDOWS
    #define WIN32_LEAN_AND_MEAN
    #include <Windows.h>
#endif

import jpt.Clock;
import jpt.DateTime;

import jpt.Debugger;

import jpt.File.Enums;
import jpt.File.IO;
import jpt.File.Path;
import jpt.File.Path.Utils;
import jpt.System.Paths;

import jpt.String.Helpers;

namespace jpt
{
    static constexpr size_t kMaxMessageSize = 1024;
    static File::Path locLogFilePath = File::Combine(File::Source::Saved, "Log.txt");

    static const char* locGetLogStr(Logger::Type type)
    {
        switch (type)
        {
            case Logger::Type::Log:            return "Log";
            case Logger::Type::Info:        return "Info";
            case Logger::Type::Warn:        return "Warn";
            case Logger::Type::Error:        return "Error";
            default:  JPT_ASSERT(false, "Unrecognized Log category"); return nullptr;
        }
    }

    void Logger::Log(Type type, int32 line, const char* file, const char* format, ...)
    {
        char messageBuffer[kMaxMessageSize];
        JPT_FORMAT_STRING(messageBuffer, format, ...);
        ProcessMessage(type, line, file, messageBuffer);
    }

    void Logger::Log(Type type, int32 line, const char* file, const wchar_t* format, ...)
    {
        wchar_t messageBuffer[kMaxMessageSize];
        JPT_FORMAT_WSTRING(messageBuffer, format, ...);
        ProcessMessage(type, line, file, messageBuffer);
    }

    String Logger::GetInfoStamp(Type type, int32 line, const char* file)
    {
        String stamp;
        stamp.Reserve(kMaxMessageSize);

        // Convert to relative path from VS proj, so double-clicking a Log message will redirect to the source code where JPT_LOG got called
        const String fileStr(file);
        stamp += ".." + File::GetSeparator<String>() + fileStr.SubStr(fileStr.Find("Source"));

        // line number and log type
        stamp += "(" + ToString(line) + "):      " + "[" + locGetLogStr(type) + "] ";

        return stamp;
    }

    String Logger::GetTimeStamp()
    {
        const DateTime now = Clock::GetCurrentDateTime();
        const String nowStr = ToString(now) + " - ";
        return nowStr;
    }

    void Logger::ProcessMessage(Type type, int32 line, const char* file, const char* pMessage)
    {
        String contentToLog = GetInfoStamp(type, line, file);
        contentToLog += pMessage;
        contentToLog += "\n";

        if (Debugger::GetInstance().IsDebuggerPresent())
        {
            SendToOutputWindow(contentToLog.ConstBuffer());
        }

        if (System::Paths::GetInstance().IsInitialized())
        {
            File::AppendTextFile(locLogFilePath, GetTimeStamp() + contentToLog);
        }
    }

    void Logger::ProcessMessage(Type type, int32 line, const char* file, const wchar_t* pMessage)
    {
        const String contentToLog = GetInfoStamp(type, line, file);
        WString ContentToLogW = StrToWStr(contentToLog);

        ContentToLogW += pMessage;
        ContentToLogW += L"\n";

        if (Debugger::GetInstance().IsDebuggerPresent())
        {
            SendToOutputWindow(ContentToLogW.ConstBuffer());
        }

        if (System::Paths::GetInstance().IsInitialized())
        {
            File::AppendTextFile(locLogFilePath, GetTimeStamp() + WStrToStr(ContentToLogW));
        }
    }

    void Logger::SendToOutputWindow(const char* string)
    {
#if IS_PLATFORM_WINDOWS
        ::OutputDebugStringA(string);
#endif
    }

    void Logger::SendToOutputWindow(const wchar_t* wideString)
    {
#if IS_PLATFORM_WINDOWS
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