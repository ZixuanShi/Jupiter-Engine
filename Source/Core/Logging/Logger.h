// Copyright Jupiter Technologies, Inc. All Rights Reserved.

#pragma once

namespace jpt
{
#if !IS_RELEASE
	#define JPT_LOG(message, ...)			{ jpt::Logger::GetInstance().Log(jpt::Logger::ELogType::Log,		__LINE__, __FILE__, message, __VA_ARGS__); }
	#define JPT_SYSTEM_INFO(message, ...)   { jpt::Logger::GetInstance().Log(jpt::Logger::ELogType::SystemInfo, __LINE__, __FILE__, message, __VA_ARGS__); }
	#define JPT_WARNING(message, ...)		{ jpt::Logger::GetInstance().Log(jpt::Logger::ELogType::Warning,	__LINE__, __FILE__, message, __VA_ARGS__); }
	#define JPT_ERROR(message, ...)			{ jpt::Logger::GetInstance().Log(jpt::Logger::ELogType::Error,		__LINE__, __FILE__, message, __VA_ARGS__); }
#else
	#define JPT_LOG(message, ...)			
	#define JPT_SYSTEM_INFO(message, ...)   
	#define JPT_WARNING(message, ...)		
	#define JPT_ERROR(message, ...)			
#endif

	/** Singleton thread-safe logger for different platforms */ 
	class JPT_API Logger
	{
	public:
		enum class ELogType
		{
			Log,			// From Client games
			SystemInfo,		// From Engine
			Warning,
			Error
		};

	public:
		/** Log a message to the terminal window
			@param type: The type to log. Choose different color to print based on the type
			@param line: The line where we called the log function
			@param file: The file where we called the log function
			@param format, ...: The message to send*/
		void Log(const ELogType& type, int32 line, const char* file, const char* format, ...);
		void Log(const char* string);
		void Log(const wchar_t* wideString);

		static Logger& GetInstance();

	private:
		Logger() = default;
	};
}