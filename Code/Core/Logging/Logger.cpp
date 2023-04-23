#include "JupiterPCH.h"
#include "Logger.h"

#include <iostream>

namespace jpt
{
	Logger Logger::s_instance;

	Logger::Logger()
#if IS_PLATFORM_WIN64
		: m_consoleHandle(GetStdHandle(STD_OUTPUT_HANDLE))
#endif
	{
	}

	void Logger::Log(ELogType type, int32 line, const char* file, const char* message, ...)
	{
		ChangeConsoleTextColor(type);

		char messageBuffer[512];

		va_list args;
		va_start(args, message);

		vsprintf_s(messageBuffer, message, args);

		va_end(args);

		// Get valid file path
		std::string fullPath(file);
		fullPath = fullPath.substr(fullPath.find("Code") + 5);

		// Log to the terminal and output window
		std::cout << fullPath << ", line (" << line << "):  \t\t" << messageBuffer << '\n';
	}

	void Logger::ChangeConsoleTextColor(ELogType type) const
	{
#if IS_PLATFORM_WIN64
		switch (type)
		{
		case jpt::Logger::ELogType::Log:			SetConsoleTextAttribute(m_consoleHandle, 7);			break;
		case jpt::Logger::ELogType::Warning:		SetConsoleTextAttribute(m_consoleHandle, 14);			break;
		case jpt::Logger::ELogType::Error:			SetConsoleTextAttribute(m_consoleHandle, 12);			break;
		default: JPT_ASSERT(false, "Unrecognized log type")			break;
		}
#endif
	}
}