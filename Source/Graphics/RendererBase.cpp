// Copyright Jupiter Technologies, Inc. All Rights Reserved.

module;

#include "Applications/AppClient.h"
#include "Applications/Window/Window.h"

module jpt.RendererBase;

import jpt.Camera;
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
        const Window& window = app.GetWindow();
        Scene& scene = app.GetScene();

        m_model = scene.GetModel().ToMatrix();
        m_viewProjection = scene.GetCamera().GetViewProjection(window.GetAspectRatio());
        m_material = scene.GetMaterial();
    }
}
