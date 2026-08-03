// Copyright Jupiter Technologies, Inc. All Rights Reserved.

module;

#include "Core/Macros.h"

export module jpt.LaunchArgs;

import jpt.TypeDefs;

export namespace jpt
{
    /** Holds what main() was given. UIApplicationMain is the only consumer today, hence no
        parsing: the format is worth choosing when something actually reads a flag. */
    class LaunchArgs
    {
    private:
        int32  m_count = 0;
        char** m_ppValues = nullptr;

    public:
        JPT_DECLARE_SINGLETON(LaunchArgs)

        void Set(int32 count, char* ppValues[]) 
        { 
            m_count = count;
            m_ppValues = ppValues; 
        }

        [[nodiscard]] int32  GetCount()  const noexcept { return m_count; }
        [[nodiscard]] char** GetValues() const noexcept { return m_ppValues; }
    };
}
