// Copyright Jupiter Technologies, Inc. All Rights Reserved.

#pragma once

import jpt.TypeDefs;
import jpt.Concepts;
import jpt.ToString;
import jpt.String;

	#define JPT_LOG(message, ...)         { jpt::Logger::GetInstance().Log(jpt::Logger::ELogType::Log,        __LINE__, __FILE__, message, __VA_ARGS__); }
	#define JPT_INFO(message, ...)        { jpt::Logger::GetInstance().Log(jpt::Logger::ELogType::Info,       __LINE__, __FILE__, message, __VA_ARGS__); }
	#define JPT_WARNING(message, ...)     { jpt::Logger::GetInstance().Log(jpt::Logger::ELogType::Warning,    __LINE__, __FILE__, message, __VA_ARGS__); }
	#define JPT_ERROR(message, ...)       { jpt::Logger::GetInstance().Log(jpt::Logger::ELogType::Error,      __LINE__, __FILE__, message, __VA_ARGS__); }

namespace jpt
{
	/** Singleton thread-safe logger for different platforms */ 
	class Logger
	{
	public:
		enum class ELogType : uint8
		{
			Log,			// From Client games
			Info,		    // From Engine
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
		template<EnabledToWString TObject>
		void Log(ELogType type, int32 line, const char* file, const TObject& obj, ...)
		{
			ProcessMessage(type, line, file, obj.ToWString().ConstBuffer());
		}
		template<Containable TContainer>
		void Log(ELogType type, int32 line, const char* file, const TContainer& container, ...)
		{
			if constexpr (AreSameType<TContainer, jpt::String> || AreSameType<TContainer, jpt::WString>)
			{
				ProcessMessage(type, line, file, container.ConstBuffer());
			}
			else
			{
				ProcessMessage(type, line, file, jpt::ToString(container).ConstBuffer());
			}
		}
		void Log(ELogType type, int32 line, const char* file, const char* format, ...);
		void Log(ELogType type, int32 line, const char* file, const wchar_t* format, ...);

		static Logger& GetInstance();

	private:
		Logger() = default;

		String GetInfoStamp(ELogType type, int32 line, const char* file);
		String GetTimeStamp();

		/** Combines stamp, message, line break, then pass to output console */
		void ProcessMessage(ELogType type, int32 line, const char* file, const char* pMessage);
		void ProcessMessage(ELogType type, int32 line, const char* file, const wchar_t* pMessage);

		/** Impl of printing message to output console */
		void SendToOutputWindow(const char* string);
		void SendToOutputWindow(const wchar_t* wideString);
	};
}