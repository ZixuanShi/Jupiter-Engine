// Copyright Jupiter Technologies, Inc. All Rights Reserved.

module jpt.LaunchArgs;

import jpt.TypeDefs;

namespace jpt
{
    void LaunchArgs::Set(int32 count, char* ppValues[])
    {
        m_count = count;
        m_ppValues = ppValues;
    }
}
