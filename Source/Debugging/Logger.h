// Copyright Jupiter Technologies, Inc. All Rights Reserved.

#pragma once

#include "Core/Validation/Assert.h"

#define IS_LOGGER_ENABLED 1

#if IS_LOGGER_ENABLED

import jpt.Concepts;

import jpt.String;
import jpt.ToString;

import jpt.TypeDefs;
import jpt.TypeTraits;

import jpt.Utilities;

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
			Warn,
			Error
		};

	public:
		/** Logs a message to the terminal window
			@param type: The type to log. Choose different color to print based on the type
			@param line: The line where we called the log function
			@param file: The file where we called the log function
			@param message, ...: Templated message to send */
		template<typename T>
		void Log(ELogType, int32, const char*, const T&, ...)
		{
			JPT_ASSERT(false, "Unsupported type");
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

#define JPT_LOG(message, ...)                                                                                                                 \
{                                                                                                                                             \
	if constexpr (jpt::IsStringLiteral<jpt::TRemoveReference<decltype(message)>>)                                                             \
	{                                                                                                                                         \
		jpt::Logger::GetInstance().Log(jpt::Logger::ELogType::Log,  __LINE__, __FILE__, message, __VA_ARGS__);                                \
	}                                                                                                                                         \
	else                                                                                                                                      \
	{                                                                                                                                         \
		jpt::Logger::GetInstance().Log(jpt::Logger::ELogType::Log,  __LINE__, __FILE__, jpt::ToString(message).ConstBuffer(), __VA_ARGS__);   \
	}                                                                                                                                         \
}																																			  
																																			  
#define JPT_INFO(message, ...)                                                                                                                \
{                                                                                                                                             \
	if constexpr (jpt::IsStringLiteral<jpt::TRemoveReference<decltype(message)>>)                                                             \
	{                                                                                                                                         \
		jpt::Logger::GetInstance().Log(jpt::Logger::ELogType::Info,  __LINE__, __FILE__, message, __VA_ARGS__);                               \
	}                                                                                                                                         \
	else                                                                                                                                      \
	{                                                                                                                                         \
		jpt::Logger::GetInstance().Log(jpt::Logger::ELogType::Info,  __LINE__, __FILE__, jpt::ToString(message).ConstBuffer(), __VA_ARGS__);  \
	}                                                                                                                                         \
}																																			  
																																			  
#define JPT_WARN(message, ...)                                                                                                                \
{                                                                                                                                             \
	if constexpr (jpt::IsStringLiteral<jpt::TRemoveReference<decltype(message)>>)                                                             \
	{                                                                                                                                         \
		jpt::Logger::GetInstance().Log(jpt::Logger::ELogType::Warn,  __LINE__, __FILE__, message, __VA_ARGS__);                               \
	}                                                                                                                                         \
	else                                                                                                                                      \
	{                                                                                                                                         \
		jpt::Logger::GetInstance().Log(jpt::Logger::ELogType::Warn,  __LINE__, __FILE__, jpt::ToString(message).ConstBuffer(), __VA_ARGS__);  \
	}                                                                                                                                         \
}

#define JPT_ERROR(message, ...)                                                                                                               \
{                                                                                                                                             \
	if constexpr (jpt::IsStringLiteral<jpt::TRemoveReference<decltype(message)>>)                                                             \
	{                                                                                                                                         \
		jpt::Logger::GetInstance().Log(jpt::Logger::ELogType::Error,  __LINE__, __FILE__, message, __VA_ARGS__);                              \
	}                                                                                                                                         \
	else                                                                                                                                      \
	{                                                                                                                                         \
		jpt::Logger::GetInstance().Log(jpt::Logger::ELogType::Error,  __LINE__, __FILE__, jpt::ToString(message).ConstBuffer(), __VA_ARGS__); \
	}                                                                                                                                         \
}

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

#define JPT_LOG_ONCE(message)     JPT_LOG_ONCE_IMPL(jpt::Logger::ELogType::Log,   message)
#define JPT_INFO_ONCE(message)    JPT_LOG_ONCE_IMPL(jpt::Logger::ELogType::Info,  message)
#define JPT_WARNING_ONCE(message) JPT_LOG_ONCE_IMPL(jpt::Logger::ELogType::Warn,  message)
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