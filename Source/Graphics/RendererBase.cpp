// Copyright Jupiter Technologies, Inc. All Rights Reserved.

module;

#include "Applications/AppClient.h"
#include "Applications/Window/Window.h"

module jpt.RendererBase;

import jpt.Camera;
import jpt.FrameTimer;
import jpt.LinearColor;
import jpt.Matrix44;
import jpt.TypeDefs;

namespace jpt
{
    bool RendererBase::PreInit() noexcept
    {
    #if IS_PLATFORM_MACOS
        m_clearColor = LinearColor(0.01f, 0.01f, 0.0f);
    #elif IS_PLATFORM_IOS
        m_clearColor = LinearColor(0.0f, 0.1f, 0.1f);
    #endif

        return true;
    }

    void RendererBase::Update()
    {
        Application& app = GetApplication();
        const FrameTimer& frameTimer = app.GetFrameTimer();
        const Window& window = app.GetWindow();
        const Camera& camera = app.GetCamera();

        const float32 elapsed = static_cast<float32>(frameTimer.GetElapsedSeconds());

        // Scene data, parked here until something owns the objects in the world.
        m_model = Mat44::RotateY(elapsed * 0.8f);
        m_viewProjection = camera.GetViewProjection(window.GetAspectRatio());
        m_time = elapsed;
    }
}
