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
        enum class Type : uint8
        {
            Log,     // From Client project
            Info,    // From Engine
            Warn,
            Error
        };

    public:
        void Log(Type type, int32 line, const char* file, const char* format, ...);
        void Log(Type type, int32 line, const char* file, const wchar_t* format, ...);

        template<typename T>
        void Log(Type, int32, const char*, const T&, ...)
        {
            JPT_ASSERT(false, "Unsupported type");
        }

        static Logger& GetInstance();

    private:
        Logger() = default;

        String GetInfoStamp(Type type, int32 line, const char* file);
        String GetTimeStamp();

        /** Combines stamp, message, line break, then pass to output console */
        void ProcessMessage(Type type, int32 line, const char* file, const char* pMessage);
        void ProcessMessage(Type type, int32 line, const char* file, const wchar_t* pMessage);

        /** Impl of printing message to output console */
        void SendToOutputWindow(const char* string);
        void SendToOutputWindow(const wchar_t* wideString);
    };
}

#define JPT_LOG(message, ...)                                                                                                                 \
{                                                                                                                                             \
    if constexpr (jpt::IsStringLiteral<jpt::TRemoveReference<decltype(message)>> ||                                                           \
                  jpt::AreSameType<decltype(message), const char*>)                                                                           \
    {                                                                                                                                         \
        jpt::Logger::GetInstance().Log(jpt::Logger::Type::Log,  __LINE__, __FILE__, message, __VA_ARGS__);                                    \
    }                                                                                                                                         \
    else                                                                                                                                      \
    {                                                                                                                                         \
        jpt::Logger::GetInstance().Log(jpt::Logger::Type::Log,  __LINE__, __FILE__, jpt::ToString(message).ConstBuffer(), __VA_ARGS__);       \
    }                                                                                                                                         \
}

#define JPT_INFO(message, ...)                                                                                                                \
{                                                                                                                                             \
    if constexpr (jpt::IsStringLiteral<jpt::TRemoveReference<decltype(message)>> ||                                                           \
                  jpt::AreSameType<decltype(message), const char*>)                                                                           \
    {                                                                                                                                         \
        jpt::Logger::GetInstance().Log(jpt::Logger::Type::Info,  __LINE__, __FILE__, message, __VA_ARGS__);                                   \
    }                                                                                                                                         \
    else                                                                                                                                      \
    {                                                                                                                                         \
        jpt::Logger::GetInstance().Log(jpt::Logger::Type::Info,  __LINE__, __FILE__, jpt::ToString(message).ConstBuffer(), __VA_ARGS__);      \
    }                                                                                                                                         \
}

#define JPT_WARN(message, ...)                                                                                                                \
{                                                                                                                                             \
    if constexpr (jpt::IsStringLiteral<jpt::TRemoveReference<decltype(message)>> ||                                                           \
                  jpt::AreSameType<decltype(message), const char*>)                                                                           \
    {                                                                                                                                         \
        jpt::Logger::GetInstance().Log(jpt::Logger::Type::Warn,  __LINE__, __FILE__, message, __VA_ARGS__);                                   \
    }                                                                                                                                         \
    else                                                                                                                                      \
    {                                                                                                                                         \
        jpt::Logger::GetInstance().Log(jpt::Logger::Type::Warn,  __LINE__, __FILE__, jpt::ToString(message).ConstBuffer(), __VA_ARGS__);      \
    }                                                                                                                                         \
}

#define JPT_ERROR(message, ...)                                                                                                               \
{                                                                                                                                             \
    if constexpr (jpt::IsStringLiteral<jpt::TRemoveReference<decltype(message)>> ||                                                           \
                  jpt::AreSameType<decltype(message), const char*>)                                                                           \
    {                                                                                                                                         \
        jpt::Logger::GetInstance().Log(jpt::Logger::Type::Error,  __LINE__, __FILE__, message, __VA_ARGS__);                                  \
    }                                                                                                                                         \
    else                                                                                                                                      \
    {                                                                                                                                         \
        jpt::Logger::GetInstance().Log(jpt::Logger::Type::Error,  __LINE__, __FILE__, jpt::ToString(message).ConstBuffer(), __VA_ARGS__);     \
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

#define JPT_LOG_ONCE(message)     JPT_LOG_ONCE_IMPL(jpt::Logger::Type::Log,   message)
#define JPT_INFO_ONCE(message)    JPT_LOG_ONCE_IMPL(jpt::Logger::Type::Info,  message)
#define JPT_WARNING_ONCE(message) JPT_LOG_ONCE_IMPL(jpt::Logger::Type::Warn,  message)
#define JPT_ERROR_ONCE(message)   JPT_LOG_ONCE_IMPL(jpt::Logger::Type::Error, message)

#else 
    template<typename... TArgs>
    void DummyLogFunction(TArgs&&...)
    {
        static_cast<void>(0);
    }

    #define JPT_LOG(message, ...)         DummyLogFunction(message, __VA_ARGS__);
    #define JPT_INFO(message, ...)         DummyLogFunction(message, __VA_ARGS__);
    #define JPT_WARNING(message, ...)      DummyLogFunction(message, __VA_ARGS__);
    #define JPT_ERROR(message, ...)        DummyLogFunction(message, __VA_ARGS__);

    #define JPT_LOG_ONCE(message)         DummyLogFunction(message);
    #define JPT_INFO_ONCE(message)        DummyLogFunction(message);
    #define JPT_WARNING_ONCE(message)     DummyLogFunction(message);
    #define JPT_ERROR_ONCE(message)       DummyLogFunction(message);
#endif