// Copyright Jupiter Technologies, Inc. All Rights Reserved.

export module jpt.RendererBase;

import jpt.LinearColor;
import jpt.Material;
import jpt.Matrix44;
import jpt.TypeDefs;

export namespace jpt
{
    /** What one frame cost. Backend-neutral: a Vulkan backend fills the same fields from a
        VkQueryPool. gpuMs arrives from the GPU after the frame that produced it has been drawn,
        so it is always one frame behind. */
    struct RenderStats
    {
        uint32  drawCalls   = 0;
        uint32  triangles   = 0;
        float64 gpuMs       = 0.0;
        float64 waitMs      = 0.0;   // Blocked waiting for a free drawable
        usize   memoryBytes = 0;
    };

    class RendererBase
    {
    protected:
        Mat44 m_model            = Mat44::Identity();
        Mat44 m_viewProjection   = Mat44::Identity();
        LinearColor m_clearColor = LinearColor::Black();
        Material m_material;
        bool m_vsync             = true;
        RenderStats m_stats;

    public:
        bool PreInit() noexcept;
        void Update();

    public:
        [[nodiscard]] const RenderStats& GetStats() const noexcept { return m_stats; }

        [[nodiscard]] const LinearColor& GetClearColor() const noexcept { return m_clearColor; }
        void SetClearColor(const LinearColor& color) noexcept { m_clearColor = color; }

        [[nodiscard]] bool IsVSyncEnabled() const noexcept { return m_vsync; }
        void SetVSync(bool enabled) noexcept { m_vsync = enabled; }
    };
}
