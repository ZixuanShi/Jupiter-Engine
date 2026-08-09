// Copyright Jupiter Technologies, Inc. All Rights Reserved.

#include "ImGuiLayer.h"

// The other half of ImGuiLayer.mm, which holds the implementation and is compiled only on Apple.
// These no-ops are what let every caller stay unguarded, and they have to live in a plain .cpp:
// as the #else of the .mm they were unreachable on any platform that compiles no Objective-C++,
// which is a link error rather than a compile one.
#if !IS_EDITOR

namespace jpt
{
    bool ImGuiInit(MTL::Device*, MTL4::CommandQueue*, int, const char*)   { return true; }
    void ImGuiInitPlatform(void*)                                         {}
    void ImGuiTerminate()                                                 {}
    void ImGuiBeginFrame(MTL4::RenderPassDescriptor*, int)                {}
    void ImGuiEndFrame(MTL4::CommandBuffer*, MTL4::RenderCommandEncoder*) {}
    void ImGuiOnPointerMoved(float, float)                                {}
    void ImGuiOnPointerButton(bool)                                       {}
    bool ImGuiWantsMouse()                                                { return false; }
    bool ImGuiWantsKeyboard()                                             { return false; }
}

#endif // !IS_EDITOR
