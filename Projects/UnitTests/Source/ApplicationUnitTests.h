// Copyright Jupiter Technologies, Inc. All Rights Reserved.

#pragma once

import jpt.ApplicationBase;

namespace jpt
{
    /** Runs every suite and quits, so a run is a verdict rather than a session -- and the verdict
        is the exit code, since a failed check returns Status::Failed.

        Init() names the four categories outright, and nothing else decides what runs: a suite is
        reached because some list calls it, so opening ApplicationUnitTests.cpp and reading down
        the tree shows the entire run. They go in Init() rather than PreInit() because
        UnitTests_SdlEvent drives a live Window, and because SetStatus is only heeded once
        ApplicationBase::Init() has set Running.

        A plain header, not a module: JPT_SYNC_APP defines jpt::GetApp(), which a plain header
        first declares, so the .cpp expanding it cannot be a module unit. */
    class ApplicationUnitTests final : public ApplicationBase
    {
    public:
        bool Init() override;
    };
}
