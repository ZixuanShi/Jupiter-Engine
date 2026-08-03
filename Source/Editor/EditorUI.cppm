// Copyright Jupiter Technologies, Inc. All Rights Reserved.

export module jpt.EditorUI;

import jpt.Camera;

export namespace jpt
{
    /** The editor's inspector. Called with the render pass already open, so it may only build
        ImGui windows -- no drawing of its own.

        Excluded from Release by CMake, along with ImGui itself, rather than compiled and left
        unreachable. Sections take what they edit by reference; lighting and post-processing
        will join Camera here. */
    void DrawEditorUI(Camera& camera);
}
