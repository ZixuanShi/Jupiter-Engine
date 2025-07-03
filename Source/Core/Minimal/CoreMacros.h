// Copyright Jupiter Technologies, Inc. All Rights Reserved.

#pragma once

import jpt.Utilities;

#pragma region Building
/** Used for hacky fix unused parameters */
#define JPT_IGNORE(...) __VA_ARGS__

/** Turn off optimization */
#define JPT_DEOPTIMIZE __pragma(optimize("", off))

/** Turn on optimization */
#define JPT_REOPTIMIZE __pragma(optimize("", on))

#pragma endregion

#pragma region Memory

/** Deleter helpers */
#define JPT_DELETE(pPointer)            \
            delete pPointer;            \
            pPointer = nullptr;

#define JPT_DELETE_ARRAY(pPointer)      \
            delete[] pPointer;          \
            pPointer = nullptr;

#define JPT_SAFE_DELETE(pPointer)       \
        if (pPointer)                   \
        {                               \
            JPT_DELETE(pPointer);       \
        }

#define JPT_SAFE_DELETE_ARRAY(pPointer) \
        if (pPointer)                   \
        {                               \
            JPT_DELETE_ARRAY(pPointer); \
        }

#define JPT_TERMINATE(pPointer)          \
        if (pPointer)                   \
        {                               \
            pPointer->Terminate();       \
            JPT_DELETE(pPointer);       \
        }

#pragma endregion

#pragma region Logging

/**< Return, with Error logging */
#define JPT_VERIFY(condition, returnValue, ...)  \
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
        return returnValue;                      \
    }                                             
                                                 
#define JPT_ENSURE(condition, ...)               \
    JPT_VERIFY(condition, false, __VA_ARGS__)    
                                                 
/**< Slient Return */                             
#define JPT_RETURN_IF(condition, returnValue)    \
    if ((condition))                             \
    {                                            \
        return returnValue;                      \
    }                                             
                                                 
#define JPT_EXIT_IF(condition)                   \
    JPT_RETURN_IF(condition, void())

#pragma endregion

#pragma region VA_ARGS

/** @return true if a macro's variadic arguments has passed in parameters. false if it's empty
    @example:
    #define MACRO_WITH_VARIADIC_ARGUMENTS(...)            { if (JPT_HAS_ARGS(__VA_ARGS__)) { DoStuff(); } }    */
#define JPT_HAS_ARGS(...) jpt::HasAnyArgs(__VA_ARGS__)

/** @return    Count of arguments in __VA_ARGS__ */
#define JPT_ARGS_COUNT(...)    jpt::GetArgsCount(__VA_ARGS__)

#pragma endregion

/** @return Count of a plain array.
    @example:
    int intArray[12];
    int intArrayCount = JPT_ARRAY_COUNT(intArray); */
template <typename T, unsigned int kNumber>
char(&ArrayCountHelper(const T(&)[kNumber]))[kNumber + 1];
#define JPT_ARRAY_COUNT(inArray) (sizeof(ArrayCountHelper(inArray)) - 1)

/** Declares a singleton class */
#define JPT_DECLARE_SINGLETON(ClassName)  \
public:                                   \
    static ClassName& GetInstance()       \
    {                                     \
        static ClassName instance;        \
        return instance;                  \
    }                                     

/** @return Offset of a member variable in a struct/class */
#define JPT_OFFSET_OF(type, member) (reinterpret_cast<size_t>(&reinterpret_cast<type*>(0)->member))