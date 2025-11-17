// Copyright Jupiter Technologies, Inc. All Rights Reserved.

#pragma once

#define ASSERT_ENABLED (!IS_CONFIG_RELEASE)

#if ASSERT_ENABLED

namespace jpt
{
    using AssertCallback = void(*)(int line, const char* file, const char* expression, const char* message);
    inline AssertCallback g_AssertCallback;

    /** Implementation when an assertion has failed
        @param line:       Line of the file caused assertion fail
        @param file:       The file where assertion failed
        @param expression: Stringified assert condition.
        @param format:     Optional. Used to append additional debug info if there's any provided. Follow printf format
        @param ... :       Optional. Variadic arguments to append to assertion. */
    void OnAssertionFailed(int line, const char* file, const char* expression, const char* format, ...);
    void OnAssertionFailed(int line, const char* file, const char* expression);

    /** Breaks the debugger if attached */
#if IS_PLATFORM_WINDOWS
    #define JPT_DEBUG_BREAK() __debugbreak()
#else
    #error "DebugBreak not implemented for this platform"
#endif        

    /** Assert with error message logging if not Release build, do nothing if Release build
        @param expression: A boolean expression to assert
        @param ...:        Error messages if the expression went false

        Example:
        int a = 10;
        int b = 11;
        JPT_ASSERT(a == 9 && b == 10);
        JPT_ASSERT(a == 9 && b == 10, "expected 9 and 10");
        JPT_ASSERT(a == 9 && b == 10, "expected 9 and 10, received %d and %d", a, b); */
#define JPT_ASSERT(expression, ...)                                                       \
            do                                                                            \
            {                                                                             \
                if (!(expression))                                                        \
                {                                                                         \
                    jpt::OnAssertionFailed(__LINE__, __FILE__, #expression, __VA_ARGS__); \
                    JPT_DEBUG_BREAK();                                                    \
                }                                                                         \
            } while(false)
}
#else
    #define JPT_ASSERT(expression, ...) static_cast<void>(0)

#endif // ASSERT_ENABLED

/**< Verify condition with Error log. Always executed regardless build config */
#define JPT_VERIFY(condition, ...)               \
    if (!(condition))                            \
    {                                            \
        if (JPT_HAS_ARGS(__VA_ARGS__))           \
        {                                        \
            JPT_ERROR("%s", __VA_ARGS__);        \
        }                                        \
        else                                     \
        {                                        \
            JPT_ERROR("Failed: %s", #condition); \
        }                                        \
    }                                             
                                                 
/** Ensure condition with Error log and early return false if failed. */
#define JPT_ENSURE(condition, ...)                                            \
    bool JPT_CONCAT(ensure, __LINE__) = (condition);                          \
    JPT_VERIFY(JPT_CONCAT(ensure, __LINE__), #condition##" - "##__VA_ARGS__); \
    if (!JPT_CONCAT(ensure, __LINE__))                                        \
    {                                                                         \
        return false;                                                         \
    }
