// Copyright Jupiter Technologies, Inc. All Rights Reserved.

// Invariants that hold *between* two independently chosen types, which is why they live here
// rather than in either one: neither header includes the other, and the assert belongs to the
// seam, not to a side of it. This translation unit deliberately compiles to nothing.

#include "Graphics/Renderer.h"

import jpt.Window;
import std;

// ApplicationBase::Init() takes the surface from the window and hands it to the renderer.
static_assert(std::is_same_v<jpt::Window::SurfaceHandle, jpt::Renderer::SurfaceHandle>, "The window's surface is not the type the renderer takes");
