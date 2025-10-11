// Copyright Jupiter Technologies, Inc. All Rights Reserved.

export module jpt.ThreadUtils;

import jpt.TypeDefs;

export namespace jpt
{
    void Sleep(int32 seconds);
    void SleepMs(int32 milliseconds);

    Index GetThreadId();
}