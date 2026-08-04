// Copyright Jupiter Technologies, Inc. All Rights Reserved.

module;

#include "Applications/AppClient.h"
#include "Applications/Window/Window.h"

module jpt.RendererBase;

import jpt.Camera;
import jpt.FrameTimer;
import jpt.LinearColor;
import jpt.Matrix44;
import jpt.Scene;
import jpt.Transform;
import jpt.TypeDefs;

namespace jpt
{
    bool RendererBase::PreInit() noexcept
    {
    #if IS_PLATFORM_MACOS
        m_clearColor = LinearColor(0.05f, 0.05f, 0.0f);
    #elif IS_PLATFORM_IOS
        m_clearColor = LinearColor(0.0f, 0.05f, 0.05f);
    #endif

        return true;
    }

    void RendererBase::Update()
    {
        Application& app = GetApplication();
        const FrameTimer& frameTimer = app.GetFrameTimer();
        const Window& window = app.GetWindow();
        Scene& scene = app.GetScene();

        m_model = scene.GetPyramid().ToMatrix();
        m_viewProjection = scene.GetCamera().GetViewProjection(window.GetAspectRatio());
        m_time = static_cast<float32>(frameTimer.GetElapsedSeconds());
    }
}
