// Copyright Jupiter Technologies, Inc. All Rights Reserved.

module;

// `stdout` is a macro, and `import std;` carries no macros.
#include <cstdio>

module jpt.Logger;

namespace jpt::Debug
{
    void PreInit()
    {
#if IS_PLATFORM_WINDOWS
        // Measured, not read from the spec: the UCRT maps _IOLBF onto _IOFBF, so a
        // newline-terminated line was still in the buffer when the process was killed. Unbuffered
        // is the only mode it honours, and the cost is one write per line on the platform that
        // draws nothing yet.
        std::setvbuf(stdout, nullptr, _IONBF, 0);
#else
        std::setvbuf(stdout, nullptr, _IOLBF, 0);
#endif
    }
}
