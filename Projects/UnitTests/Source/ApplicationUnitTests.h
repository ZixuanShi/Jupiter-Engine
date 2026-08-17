// Copyright Jupiter Technologies, Inc. All Rights Reserved.

#pragma once

import jpt.ApplicationBase;

namespace jpt
{
    /** Runs the engine's suites and quits, so a run is a verdict rather than a session.

        A plain header, not a module: JPT_SYNC_APP defines jpt::GetApp(), which a plain header
        first declares, so the .cpp expanding it cannot be a module unit. */
    class ApplicationUnitTests final : public ApplicationBase
    {
    public:
        bool PreInit(int argc, char* argv[]) override;
        bool Init() override;
    };
}
