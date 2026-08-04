// Copyright Jupiter Technologies, Inc. All Rights Reserved.

module;

#include "Applications/AppClient.h"
#include "Applications/Window/Window.h"

module jpt.RendererBase;

import jpt.Camera;
import jpt.FrameTimer;
import jpt.Light;
import jpt.LinearColor;
import jpt.Material;
import jpt.Matrix44;
import jpt.Scene;
import jpt.Transform;
import jpt.TypeDefs;

namespace jpt
{
    bool RendererBase::PreInit() noexcept
    {
        m_clearColor = LinearColor(0.0f, 0.01f, 0.01f);
        return true;
    }

    void RendererBase::Update()
    {
        Application& app = GetApplication();
        const FrameTimer& frameTimer = app.GetFrameTimer();
        const Window& window = app.GetWindow();
        Scene& scene = app.GetScene();

        m_model = scene.GetModel().ToMatrix();
        m_viewProjection = scene.GetCamera().GetViewProjection(window.GetAspectRatio());
        m_cameraPosition = scene.GetCamera().GetPosition();
        m_material = scene.GetMaterial();
        m_ambient = scene.GetAmbient();
        m_pointLights = scene.GetPointLights();
        m_time = static_cast<float32>(frameTimer.GetElapsedSeconds());
    }
}
