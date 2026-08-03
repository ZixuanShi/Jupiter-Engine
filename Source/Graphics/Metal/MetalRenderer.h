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
        /** What this backend needs to bind a surface. RendererType reads it off the type, so
            the seam stays fully typed: VulkanRenderer would declare VkSurfaceKHR here. */
        using SurfaceHandle = CA::MetalLayer*;

        /** Device and queue need no surface, so they are created before a window exists.
            This is where Vulkan will create its instance and device. */
        bool PreInit();
        bool Init(SurfaceHandle pMetalLayer);
        void Terminate();

    public:
        void OnResize(uint32 pixelWidth, uint32 pixelHeight);
        void OnFrameDraw(float64 elapsedSeconds);
    };
}
