// Copyright Jupiter Technologies, Inc. All Rights Reserved.

#pragma once

#if !IS_RELEASE

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
		template<NoBuiltInToStringPrimitive TPrimitiveData>
		void Log(ELogType type, int32 line, const char* file, TPrimitiveData data, ...)
		{
			ProcessMessage(type, line, file, jpt::ToString(data).ConstBuffer());
		}
		template<EnabledToString TObject>
		void Log(ELogType type, int32 line, const char* file, const TObject& obj, ...)
		{
			ProcessMessage(type, line, file, obj.ToString().ConstBuffer());
		}
		template<Containable TContainer>
		void Log(ELogType type, int32 line, const char* file, const TContainer& container, ...)
		{
			ProcessMessage(type, line, file, jpt::ToString(container).ConstBuffer());
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
	#define JPT_LOG(message, ...)	      static_cast<void>(message)		
	#define JPT_SYSTEM_INFO(message, ...) static_cast<void>(message)	
	#define JPT_WARNING(message, ...)     static_cast<void>(message)	
	#define JPT_ERROR(message, ...)		  static_cast<void>(message)	

#endif // !IS_RELEASE