// Copyright Jupiter Technologies, Inc. All Rights Reserved.

#pragma once

#include "Graphics/GraphicsConstants.h"
#include "GpuCapture.h"

import jpt.Mesh;
import jpt.RendererBase;
import jpt.Texture;
import jpt.TypeDefs;
import std;

// Forward declared so this header carries no metal-cpp dependency. The cost is that
// NS::SharedPtr is unavailable, so Terminate() releases what Init() owns.
namespace MTL  
{ 
    class Device; 
    class RenderPipelineState; 
    class DepthStencilState; 
    class Texture; 
    class Buffer; 
    class ResidencySet; 
    class SamplerState; 
}

namespace MTL4 
{ 
    class CommandQueue; 
    class CommandAllocator; 
    class CommandBuffer; 
    class RenderPassDescriptor; 
    class ArgumentTable; 
    class Compiler; 
}

namespace CA   
{ 
    class MetalLayer; 
    class MetalDrawable; 
}

namespace NS   
{ 
    class AutoreleasePool; 
}

namespace jpt
{
    class Metal4Renderer : public RendererBase
    {
    public:
        using SurfaceHandle = CA::MetalLayer*;

    private:
        CA::MetalLayer*           m_pLayer         = nullptr;   // Borrowed from the view
        MTL::Device*              m_pDevice        = nullptr;
        MTL4::CommandQueue*       m_pQueue         = nullptr;
        MTL4::Compiler*           m_pCompiler      = nullptr;
        MTL::RenderPipelineState* m_pPipeline      = nullptr;
        MTL::DepthStencilState*   m_pDepthState    = nullptr;
        MTL::DepthStencilState*   m_pUIDepthState  = nullptr;   // Depth off, for UI drawn over the scene
        MTL::Texture*             m_pMsaaColor     = nullptr;   // Rendered into, then resolved to the drawable
        MTL::Texture*             m_pDepthTexture  = nullptr;
        MTL::Buffer*              m_pVertices      = nullptr;
        MTL::Buffer*              m_pIndices       = nullptr;
        MTL::Buffer*              m_pUniforms      = nullptr;   // kFramesInFlight slots, one per frame
        MTL4::ArgumentTable*      m_pArgumentTable = nullptr;
        MTL::ResidencySet*        m_pResidencySet  = nullptr;
        MTL::SamplerState*        m_pSampler       = nullptr;

        MTL::Texture* m_pTextures[kTextureSlotCount] = {};

        // One per frame in flight, because reset() frees the commands of every buffer it has
        // allocated -- the GPU must be done with slot i before frame i+kFramesInFlight resets it.
        MTL4::CommandAllocator* m_pAllocators[kFramesInFlight] = {};

        // Holds the texture upload commands, which outlive the call that recorded them.
        MTL4::CommandAllocator* m_pUploadAllocator = nullptr;

        uint32 m_frameIndex = 0;
        uint32 m_indexCount = 0;

        // Written by the commit feedback handler, which Metal runs on its own thread.
        std::atomic<float64> m_gpuMilliseconds{ 0.0 };

        // Taken in BeginFrame, published into m_stats at EndFrame -- see the comment there.
        float64 m_waitMilliseconds = 0.0;

        GpuCapture m_capture;

        // Live only between BeginFrame and EndFrame, and owned by that frame's autorelease pool
        // rather than by this object -- so EndFrame nulls them and releases nothing but the pool.
        NS::AutoreleasePool*        m_pFramePool     = nullptr;
        CA::MetalDrawable*          m_pDrawable      = nullptr;
        MTL4::RenderPassDescriptor* m_pPass          = nullptr;
        MTL4::CommandBuffer*        m_pCommandBuffer = nullptr;

    public:
        bool PreInit();
        bool Init(SurfaceHandle pMetalLayer);
        void Terminate();

        bool BeginFrame();
        void EndFrame();

    public:
        void OnResize(uint32 pixelWidth, uint32 pixelHeight);
        void SetVSync(bool enabled) noexcept;

        /** Writes the next frame to GetSavedDir()/Traces as a .gputrace, to open in Xcode. */
        void RequestCapture();
        void DeleteCaptures();

        bool SetMesh(const Mesh& mesh);

        /** One per TextureSlot, in slot order. All of them at once, so a single command buffer
            generates every mip chain. */
        bool SetTextures(std::span<const Texture> textures);

    private:
        bool CreatePipeline();
        bool CreateSampler();
        bool EnsureFrameTextures(uint32 pixelWidth, uint32 pixelHeight);
        void GenerateMipmaps();

        /** Metal 4 does not track what a GPU address points at, so everything reached that way has
            to be declared resident by hand. Rebuilt whenever one of those allocations changes. */
        void UpdateResidency();

        /** Drains the frame's autorelease pool, returns the frame slot, and forgets what the pool
            owned. Every path out of BeginFrame goes through it, including the failures. */
        void EndFramePool();
    };
}
