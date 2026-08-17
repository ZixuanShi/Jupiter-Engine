// Copyright Jupiter Technologies, Inc. All Rights Reserved.

#pragma once

import jpt.ApplicationBase;

namespace jpt
{
    /** Runs every registered suite and quits, so a run is a verdict rather than a session -- and
        the verdict is the exit code, since a failed check returns Status::Failed.

        It names no suite: each registers itself from its own file during static initialisation,
        so adding one is dropping a file under Source/. They run from Init() rather than PreInit()
        because SdlEventTests drives a live Window, and because SetStatus is only heeded once
        ApplicationBase::Init() has set Running.

        A plain header, not a module: JPT_SYNC_APP defines jpt::GetApp(), which a plain header
        first declares, so the .cpp expanding it cannot be a module unit. */
    class ApplicationUnitTests final : public ApplicationBase
    {
    public:
        bool Init() override;
    };
}
