#pragma once

namespace jpt
{
	#define JPT_LOG(message, ...)     { jpt::Logger::GetInstance()->Log(jpt::Logger::ELogType::Log,     __LINE__, __FILE__, message, __VA_ARGS__); }
	#define JPT_WARNING(message, ...) { jpt::Logger::GetInstance()->Log(jpt::Logger::ELogType::Warning, __LINE__, __FILE__, message, __VA_ARGS__); }
	#define JPT_ERROR(message, ...)   { jpt::Logger::GetInstance()->Log(jpt::Logger::ELogType::Error,   __LINE__, __FILE__, message, __VA_ARGS__); }

	// Singleton logger
	class JPT_API Logger
	{
	public:
		enum class ELogType : uint8
		{
			Log,		// Normal message prompt
			Warning,
			Error,
		};

	public:
		// Log a message to the terminal window
		// - type: The type to log. Choose different color to print based on the type
		// - line: The line where we called the log function
		// - file: The file where we called the log function
		// - format, ...: The message to send
		void Log(ELogType type, int32 line, const char* file, const char* format, ...);

		static Logger* GetInstance() { return &s_instance; }

	private:
		Logger();

		void ChangeConsoleTextColor(ELogType type) const;

	private:
		static Logger s_instance;

#if IS_PLATFORM_WIN64
		HANDLE m_consoleHandle;
#endif
	};
}