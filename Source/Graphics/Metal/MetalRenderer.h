// Copyright Jupiter Technologies, Inc. All Rights Reserved.

#pragma once

import jpt.Mesh;
import jpt.RendererBase;
import jpt.TypeDefs;

// Forward declared so this header carries no metal-cpp dependency. The cost is that
// NS::SharedPtr is unavailable, so Terminate() releases what Init() owns.
namespace MTL { class Device; class CommandQueue; class RenderPipelineState; class DepthStencilState; class Texture; class Buffer; }
namespace CA  { class MetalLayer; }

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
        MTL::Texture*             m_pDepthTexture = nullptr;
        MTL::Buffer*              m_pVertices     = nullptr;
        MTL::Buffer*              m_pIndices      = nullptr;
        CA::MetalLayer*           m_pLayer        = nullptr;   // Borrowed from the view
        uint32 m_indexCount = 0;

    public:
        bool PreInit();
        bool Init(SurfaceHandle pMetalLayer);
        void Terminate();

    public:
        bool SetMesh(const Mesh& mesh);
        void OnResize(uint32 pixelWidth, uint32 pixelHeight);
        void Draw();

    private:
        bool CreatePipeline();
        bool EnsureDepthTexture(uint32 pixelWidth, uint32 pixelHeight);
    };
}
