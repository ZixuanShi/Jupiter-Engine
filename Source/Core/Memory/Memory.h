#pragma once

#include <type_traits>

#if !IS_CONFIG_RELEASE

import jpt.TypeTraits;

#define JPT_NEW(type, ...) jpt::Allocator<type>::New(__VA_ARGS__)

#define JPT_NEW_ARRAY(type, count, ...) jpt::Allocator<type>::NewArray(count, __VA_ARGS__)

#define JPT_DELETE(pPointer)                                                           \
                jpt::Allocator<std::remove_pointer_t<decltype(pPointer)>>::Delete(pPointer); \
                pPointer = nullptr;

#define JPT_DELETE_ARRAY(pPointer)                                                          \
                jpt::Allocator<std::remove_pointer_t<decltype(pPointer)>>::DeleteArray(pPointer); \
                pPointer = nullptr;
#else

import jpt.Allocator;

#define JPT_NEW(type, ...) new type(__VA_ARGS__)

#define JPT_NEW_ARRAY(type, count, ...) new type[count] { __VA_ARGS__ }

#define JPT_DELETE(pPointer)        \
                delete pPointer;        \
                pPointer = nullptr;    

#define JPT_DELETE_ARRAY(pPointer)  \
                delete[] pPointer;      \
                pPointer = nullptr;
#endif

#define JPT_TERMINATE(pPointer)         \
        if (pPointer)                   \
        {                               \
            pPointer->Terminate();      \
            JPT_DELETE(pPointer);       \
        }