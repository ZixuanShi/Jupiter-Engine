// Copyright Jupiter Technologies, Inc. All Rights Reserved.

module;

#include "Core/Validation/Assert.h"

export module jpt.AssertImpl;

#if ASSERT_ENABLED
export namespace jpt
{
    class AssertImpl
    {
    public:
        bool PreInit();
    };
}
#endif