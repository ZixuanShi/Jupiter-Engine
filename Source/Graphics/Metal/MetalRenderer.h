// Copyright Jupiter Technologies, Inc. All Rights Reserved.

#pragma once

import jpt.TypeDefs;

// Forward declared so this header carries no metal-cpp dependency. The cost is that
// NS::SharedPtr is unavailable, so Terminate() releases what Init() owns.
namespace MTL { class Device; class CommandQueue; }
namespace CA  { class MetalLayer; }

namespace jpt
{
    class MetalRenderer
    {
    private:
        MTL::Device*       m_pDevice = nullptr;
        MTL::CommandQueue* m_pQueue  = nullptr;
        CA::MetalLayer*    m_pLayer  = nullptr;   // Borrowed from the view

    public:
        /** Takes a CAMetalLayer* as an opaque handle so the signature stays ObjC-free. */
        bool Init(void* pMetalLayer);

        /** Sizes are in pixels, not points. */
        void OnResize(uint32 pixelWidth, uint32 pixelHeight);

        void OnFrameDraw(float64 elapsedSeconds);
        void Terminate();
    };
}
