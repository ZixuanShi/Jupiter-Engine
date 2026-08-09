// Copyright Jupiter Technologies, Inc. All Rights Reserved.

#pragma once

import jpt.Mesh;
import jpt.RendererBase;
import jpt.Texture;
import jpt.TypeDefs;

namespace jpt
{
    /** A renderer that draws nothing, so a platform with no backend yet still links and runs.

        BeginFrame() always declines, which is the same answer a real backend gives for a frame
        it cannot start -- so ApplicationBase::OnFrame() takes a path it already handles rather
        than one only this type produces. Everything the scene feeds it still runs: Update() is
        RendererBase's, and it pulls the camera and model matrices every frame.

        Replaced by RendererVulkan, not extended. */
    class RendererNull : public RendererBase
    {
    public:
        // void* rather than a real handle: nothing here dereferences it, and it keeps the
        // OnSurfaceReady signature that leaks out to project code honest about carrying nothing.
        using SurfaceHandle = void*;

    public:
        bool PreInit();
        bool Init(SurfaceHandle surface);
        void Terminate();

        bool BeginFrame();
        void EndFrame();

    public:
        void OnResize(uint32 pixelWidth, uint32 pixelHeight);

        void RequestCapture();
        void DeleteCaptures();

        bool SetMesh(const Mesh& mesh);
        bool SetTexture(const Texture& texture);
    };
}
