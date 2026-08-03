// Copyright Jupiter Technologies, Inc. All Rights Reserved.

#pragma once

/** Lazily constructed on first use, destroyed at exit. Thread-safe since C++11. */
#define JPT_DECLARE_SINGLETON(ClassName)     \
    static ClassName& GetInstance()          \
    {                                        \
        static ClassName s_instance;         \
        return s_instance;                   \
    }
