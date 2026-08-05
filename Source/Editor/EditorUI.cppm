// Copyright Jupiter Technologies, Inc. All Rights Reserved.

export module jpt.EditorUI;

import jpt.TypeDefs;

export namespace jpt
{
    /** The editor's inspector. Draw() runs with the render pass already open, so it may only
        build ImGui windows -- nothing here draws.

        Excluded from Release by CMake, along with ImGui itself, rather than compiled and left
        unreachable. Every section reaches what it edits through GetApplication(), the same way
        subsystems do, which is why none of these take parameters and why this unit needs no
        import of Camera, Material or PointLight. */
    class EditorUI
    {
    private:
        bool m_showMetrics = true;

    public:
        void Draw();

    private:
        void DrawPerformance();
        void DrawRendering();
        void DrawPointLight(usize index);
        void DrawPointLightGizmos();
        void DrawCamera();

        /** Every metric, in its own corner window rather than in the panel, so the numbers stay
            visible while the Dev Menu is collapsed or dragged away. */
        void DrawOverlay();
    };
}
