// Copyright Jupiter Technologies, Inc. All Rights Reserved.

module;

#include "Applications/AppClient.h"

module jpt.Scene;

import jpt.FrameTimer;
import jpt.Quaternion;
import jpt.TypeDefs;
import jpt.Vector3;

namespace jpt
{
    void Scene::Update()
    {
        const FrameTimer& frameTimer = GetApplication().GetFrameTimer();
        const float32 elapsed = static_cast<float32>(frameTimer.GetElapsedSeconds());

        // Placeholder motion, moved here from the renderer. Input replaces it next.
        m_pyramid.rotation = Quat::FromAxisAngle(Vec3::Up(), elapsed * 0.8f);
    }
}
