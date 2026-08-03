// Copyright Jupiter Technologies, Inc. All Rights Reserved.

#pragma once

import jpt.Mesh;
import jpt.RendererBase;
import jpt.TypeDefs;

// Forward declared so this header carries no metal-cpp dependency. The cost is that
// NS::SharedPtr is unavailable, so Terminate() releases what Init() owns.
namespace MTL { class Device; class CommandQueue; class RenderPipelineState; class DepthStencilState; class Texture; class Buffer; class RenderPassDescriptor; }
namespace CA  { class MetalLayer; class MetalDrawable; }
namespace NS  { class AutoreleasePool; }

namespace jpt
{
    class MetalRenderer : public RendererBase
    {
    public:
        using SurfaceHandle = CA::MetalLayer*;

    private:
        MTL::Device*              m_pDevice       = nullptr;
        MTL::CommandQueue*        m_pQueue        = nullptr;
        MTL::RenderPipelineState* m_pPipeline     = nullptr;
        MTL::DepthStencilState*   m_pDepthState   = nullptr;
        MTL::DepthStencilState*   m_pUIDepthState = nullptr;   // Depth off, for UI drawn over the scene
        MTL::Texture*             m_pDepthTexture = nullptr;
        MTL::Buffer*              m_pVertices     = nullptr;
        MTL::Buffer*              m_pIndices      = nullptr;
        CA::MetalLayer*           m_pLayer        = nullptr;   // Borrowed from the view
        uint32 m_indexCount = 0;

        // Live only between BeginFrame and EndFrame, and owned by that frame's autorelease pool
        // rather than by this object -- so EndFrame nulls them and releases nothing but the pool.
        NS::AutoreleasePool*       m_pFramePool = nullptr;
        CA::MetalDrawable*         m_pDrawable  = nullptr;
        MTL::RenderPassDescriptor* m_pPass      = nullptr;

    public:
        bool PreInit();
        bool Init(SurfaceHandle pMetalLayer);
        void Terminate();

    public:
        /** Acquires the drawable and opens the pass. False when no drawable was free, in which
            case EndFrame must not be called -- the frame is simply dropped. */
        bool BeginFrame();
        void EndFrame();

    public:
        void OnResize(uint32 pixelWidth, uint32 pixelHeight);

    public:
        bool SetMesh(const Mesh& mesh);

    private:
        bool CreatePipeline();
        bool EnsureDepthTexture(uint32 pixelWidth, uint32 pixelHeight);

        /** Drains the frame's autorelease pool and forgets what it owned. Every path out of
            BeginFrame goes through it, including the failures. */
        void EndFramePool();
    };
}
