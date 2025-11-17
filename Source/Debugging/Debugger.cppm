// Copyright Jupiter Technologies, Inc. All Rights Reserved.

module;

#include "Core/Minimal/Utilities.h"
#include "Core/Validation/Assert.h"

export module jpt.Debugger;

import jpt.AssertImpl;

export namespace jpt
{
    class Debugger
    {
        JPT_DECLARE_SINGLETON(Debugger);

    private:
#if ASSERT_ENABLED
        AssertImpl m_assertImpl;
#endif

    public:
        bool PreInit();

    public:
        bool IsDebuggerPresent() const;
    };
}