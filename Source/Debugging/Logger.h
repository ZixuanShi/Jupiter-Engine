// Copyright Jupiter Technologies, Inc. All Rights Reserved.

#pragma once

#if JPT_ENABLE_EDITOR_FEATURES

import jpt.TypeDefs;
import jpt.Concepts;
import jpt.ToString;
import jpt.String;

	#define JPT_LOG(message, ...)			{ jpt::Logger::GetInstance().Log(jpt::Logger::ELogType::Log,		__LINE__, __FILE__, message, __VA_ARGS__); }
	#define JPT_SYSTEM_INFO(message, ...)   { jpt::Logger::GetInstance().Log(jpt::Logger::ELogType::SystemInfo, __LINE__, __FILE__, message, __VA_ARGS__); }
	#define JPT_WARNING(message, ...)		{ jpt::Logger::GetInstance().Log(jpt::Logger::ELogType::Warning,	__LINE__, __FILE__, message, __VA_ARGS__); }
	#define JPT_ERROR(message, ...)			{ jpt::Logger::GetInstance().Log(jpt::Logger::ELogType::Error,		__LINE__, __FILE__, message, __VA_ARGS__); }

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
		template<NoBuiltInToString MessageT>
		void Log(ELogType type, int32 line, const char* file, MessageT message, ...)
		{
			ProcessMessage(type, line, file, jpt::ToString(message).ConstBuffer());
		}
		template<EnabledToString ToTString>
		void Log(ELogType type, int32 line, const char* file, const ToTString& obj, ...)
		{
			ProcessMessage(type, line, file, obj.ToString().ConstBuffer());
		}
		void Log(ELogType type, int32 line, const char* file, const char* format, ...);
		void Log(ELogType type, int32 line, const char* file, const wchar_t* format, ...);

		static Logger& GetInstance();

	private:
		Logger() = default;

		/** @return	A time stamp that follows the format of filePath(lineNumber): 
			This allows LOG message clickable in the output window, and will redirect to the source code where LOG is called 
			@note filePath can be either relative to the VS proj file, or absolute path. Using relative path as default */
		String GetStamp(ELogType type, int32 line, const char* file);

		/** Combines stamp, message, line break, then pass to output console */
		void ProcessMessage(ELogType type, int32 line, const char* file, const char* pMessage);
		void ProcessMessage(ELogType type, int32 line, const char* file, const wchar_t* pMessage);

		/** Impl of printing message to output console */
		void SendToOutputWindow(const char* string);
		void SendToOutputWindow(const wchar_t* wideString);
	};
}
#else
	#define JPT_LOG(message, ...)			
	#define JPT_SYSTEM_INFO(message, ...)   
	#define JPT_WARNING(message, ...)		
	#define JPT_ERROR(message, ...)		

#endif // JPT_ENABLE_EDITOR_FEATURES