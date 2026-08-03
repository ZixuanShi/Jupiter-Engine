// Copyright Jupiter Technologies, Inc. All Rights Reserved.

#pragma once

import jpt.TypeDefs;

// Forward declared so this header carries no metal-cpp dependency. The cost is that
// NS::SharedPtr is unavailable, so Terminate() releases what Init() owns.
namespace MTL { class Device; class CommandQueue; class RenderPipelineState; class Buffer; }
namespace CA  { class MetalLayer; }

namespace jpt
{
    class MetalRenderer
    {
    public:
        using SurfaceHandle = CA::MetalLayer*;

    private:
        MTL::Device*              m_pDevice   = nullptr;
        MTL::CommandQueue*        m_pQueue    = nullptr;
        MTL::RenderPipelineState* m_pPipeline = nullptr;
        MTL::Buffer*              m_pVertices = nullptr;
        CA::MetalLayer*           m_pLayer    = nullptr;   // Borrowed from the view

    public:
        bool PreInit();
        bool Init(SurfaceHandle pMetalLayer);
        void Terminate();

    public:
        void OnResize(uint32 pixelWidth, uint32 pixelHeight);
        void OnFrameDraw(float64 elapsedSeconds);

    private:
        bool CreatePipeline();
        bool CreateGeometry();
    };
}
