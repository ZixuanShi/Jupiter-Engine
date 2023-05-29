#pragma once

#include <iostream>

namespace jpt
{
#if !IS_RELEASE
	#define JPT_LOG(message, ...)			{ jpt::Logger::GetInstance().Log(jpt::Logger::ELogType::Log,		__LINE__, __FILE__, message, __VA_ARGS__); }
	#define JPT_WARNING(message, ...)		{ jpt::Logger::GetInstance().Log(jpt::Logger::ELogType::Warning,	__LINE__, __FILE__, message, __VA_ARGS__); }
	#define JPT_ERROR(message, ...)			{ jpt::Logger::GetInstance().Log(jpt::Logger::ELogType::Error,		__LINE__, __FILE__, message, __VA_ARGS__); }
	#define JPT_SYSTEM_INFO(message, ...)   { jpt::Logger::GetInstance().Log(jpt::Logger::ELogType::SystemInfo, __LINE__, __FILE__, message, __VA_ARGS__); }
#endif

	// Singleton logger
	class JPT_API Logger
	{
	public:
		// Value as the text color to print
		enum class ELogType : uint8
		{
			Log			= 7,	// From Client games
			SystemInfo	= 3,    // From Engine
			Warning		= 6,
			Error		= 4,
		};

	private:
		static Logger s_instance;

#if IS_PLATFORM_WIN64
		HANDLE m_consoleHandle;
#endif

	public:
		// Log a message to the terminal window
		// - type: The type to log. Choose different color to print based on the type
		// - line: The line where we called the log function
		// - file: The file where we called the log function
		// - format, ...: The message to send
		void Log(ELogType type, int32 line, const char* file, const char* format, ...);

		// Templated version of print, where users define the cout operator overloadings
		template<class... Args>
		void Print(Args&&... args) const;

		static Logger& GetInstance() { return s_instance; }

	private:
		Logger();

		void ChangeConsoleTextColor(ELogType type) const;
	};

	template<class... Args>
	void Logger::Print(Args&&... args) const
	{
		(std::cout << ... << args) << '\n';
	}
}