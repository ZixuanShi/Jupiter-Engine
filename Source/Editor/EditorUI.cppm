// Copyright Jupiter Technologies, Inc. All Rights Reserved.

export module jpt.EditorUI;

export namespace jpt
{
    /** The editor's inspector. Called with the render pass already open, so it may only build
        ImGui windows -- no drawing of its own.

        Excluded from Release by CMake, along with ImGui itself, rather than compiled and left
        unreachable. Takes nothing: sections reach what they edit through GetApplication(), the
        same way subsystems do, so adding one costs no signature change here. */
    void DrawEditorUI();
}
