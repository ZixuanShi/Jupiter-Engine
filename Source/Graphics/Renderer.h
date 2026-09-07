// Copyright Jupiter Technologies, Inc. All Rights Reserved.

#pragma once

#if IS_PLATFORM_MACOS || IS_PLATFORM_IOS
    #include "Metal/RendererMetal4.h"
#else
    #include "Null/RendererNull.h"  // Vulkan later
#endif

import std;

namespace jpt
{
#if IS_PLATFORM_MACOS || IS_PLATFORM_IOS
    using Renderer = RendererMetal4;
#else
    using Renderer = RendererNull;
#endif

    static_assert(!std::is_polymorphic_v<Renderer>, "Renderer must stay vtable-free");
}
