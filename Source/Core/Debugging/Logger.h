// Copyright Jupiter Technologies, Inc. All Rights Reserved.

#pragma once

import jpt.TypeDefs;
import jpt.Concepts;
import jpt.String;

#if IS_DEBUG
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

#if IS_DEBUG
namespace jpt
{
	/** Singleton thread-safe logger for different platforms */ 
	class Logger
	{
	public:
		enum class ELogType : uint8
		{
			Log,			// From Client games
			SystemInfo,		// From Engine
			Warning,
			Error
		};

	public:
		/** Logs a message to the terminal window
			@param type: The type to log. Choose different color to print based on the type
			@param line: The line where we called the log function
			@param file: The file where we called the log function
			@param message, ...: Templated message to send */
		template<NoBuiltInToStringPrimitive MessageT>
		void Log(ELogType type, int32 line, const char* file, MessageT message, ...)
		{
			ProcessMessage(type, line, file, jpt::ToString(message).ConstBuffer());
		}
		template<EnabledToString ToStringT>
		void Log(ELogType type, int32 line, const char* file, const ToStringT& obj, ...)
		{
			ProcessMessage(type, line, file, obj.ToString().ConstBuffer());
		}
		template<BasicStringType StringT>
		void Log(ELogType type, int32 line, const char* file, const StringT& string, ...)
		{
			ProcessMessage(type, line, file, string.ConstBuffer());
		}
		void Log(ELogType type, int32 line, const char* file, const char* message, ...);

		static Logger& GetInstance();

	private:
		Logger() = default;

		void ProcessMessage(ELogType type, int32 line, const char* file, const char* pMessage);
		void PrintToConsole(const char* string);
		void PrintToConsole(const wchar_t* wideString);
	};
}
#endif