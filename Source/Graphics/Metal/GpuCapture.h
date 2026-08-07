// Copyright Jupiter Technologies, Inc. All Rights Reserved.

#pragma once

import jpt.TypeDefs;
import std;

namespace MTL
{
    class Device;
}

namespace jpt
{
    /** One frame written as a .gputrace under GetSavedDir()/Traces, to open in Xcode. Programmatic
        because the build is Ninja and there is no scheme to press Xcode's capture button in.

        Metal only allows this when the bundle carries MTLCaptureEnabled, which CMake writes into
        the Info.plist for every config but Release. */
    class GpuCapture
    {
    private:
        std::filesystem::path m_path;   // Empty unless a capture is scheduled
        uint32 m_framesUntil = 0;

    public:
        /** Captures the next frame that renders. Requesting during a frame is the normal case --
            the Dev Menu draws inside one -- so it can never be this frame. */
        void RequestCapture();

        /** Deletes every capture under GetSavedDir()/Traces. In-app because it is the only
            actor that can delete inside the iOS sandbox: devicectl has no delete command, and
            its --remove-existing-content flag wipes the whole app container (measured). */
        void DeleteAll();

        void BeginFrame(MTL::Device* pDevice);
        void EndFrame();
    };
}
