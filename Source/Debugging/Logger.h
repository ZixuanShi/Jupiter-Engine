// Copyright Jupiter Technologies, Inc. All Rights Reserved.

#pragma once

#define IS_LOGGER_ENABLED 1

#if IS_LOGGER_ENABLED

import jpt.TypeDefs;
import jpt.Concepts;
import jpt.ToString;
import jpt.String;

namespace jpt
{
	/** Singleton thread-safe logger for different platforms */ 
	class Logger
	{
	public:
		enum class ELogType : uint8
		{
			Log,			// From Client project
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

#define JPT_LOG(message, ...)          { jpt::Logger::GetInstance().Log(jpt::Logger::ELogType::Log,        __LINE__, __FILE__, message, __VA_ARGS__); }
#define JPT_INFO(message, ...)         { jpt::Logger::GetInstance().Log(jpt::Logger::ELogType::Info,       __LINE__, __FILE__, message, __VA_ARGS__); }
#define JPT_WARNING(message, ...)      { jpt::Logger::GetInstance().Log(jpt::Logger::ELogType::Warning,    __LINE__, __FILE__, message, __VA_ARGS__); }
#define JPT_ERROR(message, ...)        { jpt::Logger::GetInstance().Log(jpt::Logger::ELogType::Error,      __LINE__, __FILE__, message, __VA_ARGS__); }

/** Log only once,
	if the message is the same as the last time, it will not log again
	if the message is different, it will log again
	@param type    The type of log (Log, Info, Warning, Error)
	@param message Expects single object or formatted string */
#define JPT_LOG_ONCE_IMPL(type, message)        \
{                                               \
    static auto s_copy = message;               \
    static bool s_hasLogged = false;            \
    if (!s_hasLogged || s_copy != message)      \
    {                                           \
        s_copy = message;                       \
        s_hasLogged = true;                     \
        jpt::Logger::GetInstance().Log(         \
            type, __LINE__, __FILE__, message); \
    }                                           \
}

#define JPT_LOG_ONCE(message)     JPT_LOG_ONCE_IMPL(jpt::Logger::ELogType::Log, message)
#define JPT_INFO_ONCE(message)    JPT_LOG_ONCE_IMPL(jpt::Logger::ELogType::Info, message)
#define JPT_WARNING_ONCE(message) JPT_LOG_ONCE_IMPL(jpt::Logger::ELogType::Warning, message)
#define JPT_ERROR_ONCE(message)   JPT_LOG_ONCE_IMPL(jpt::Logger::ELogType::Error, message)

#else 
	template<typename... TArgs>
	void DummyLogFunction(TArgs&&...)
	{
		static_cast<void>(0);
	}

	#define JPT_LOG(message, ...)         DummyLogFunction(message, __VA_ARGS__);
	#define JPT_INFO(message, ...)   	  DummyLogFunction(message, __VA_ARGS__);
	#define JPT_WARNING(message, ...)	  DummyLogFunction(message, __VA_ARGS__);
	#define JPT_ERROR(message, ...)  	  DummyLogFunction(message, __VA_ARGS__);

	#define JPT_LOG_ONCE(message)         DummyLogFunction(message);
	#define JPT_INFO_ONCE(message)        DummyLogFunction(message);
	#define JPT_WARNING_ONCE(message)     DummyLogFunction(message);
	#define JPT_ERROR_ONCE(message)       DummyLogFunction(message);
#endif